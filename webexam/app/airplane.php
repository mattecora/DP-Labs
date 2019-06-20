<?php
    /*
        airplane.php
        Provides the facade class for the application, containing the DB interaction
        Matteo Corain - Distributed programming I - A.Y. 2018-19
    */

    require_once "dbconf.php";
    require_once "message.php";
    require_once "seat.php";
    require_once "seatmap.php";
    require_once "session.php";

    class Airplane {
        const STMT_GET_SEAT = 0;
        const STMT_GET_SEAT_FOR_UPDATE = 1;
        const STMT_GET_SEAT_ALL = 2;
        const STMT_ADD_RESERVATION = 3;
        const STMT_UPD_RESERVATION = 4;
        const STMT_DEL_RESERVATION = 5;
        const STMT_ADD_USER = 6;
        const STMT_CHECK_PASSWORD = 7;

        const STMT_SQL_CODE = array(
            Airplane::STMT_GET_SEAT => "SELECT status, reserver FROM airplane WHERE seat = ?",
            Airplane::STMT_GET_SEAT_FOR_UPDATE => "SELECT status, reserver FROM airplane WHERE seat = ? FOR UPDATE",
            Airplane::STMT_GET_SEAT_ALL => "SELECT * FROM airplane",
            Airplane::STMT_ADD_RESERVATION => "INSERT INTO airplane VALUES (?, ?, ?)",
            Airplane::STMT_UPD_RESERVATION => "UPDATE airplane SET status = ?, reserver = ? WHERE seat = ?",
            Airplane::STMT_DEL_RESERVATION => "DELETE FROM airplane WHERE seat = ?",
            Airplane::STMT_ADD_USER => "INSERT INTO users VALUES (?, ?)",
            Airplane::STMT_CHECK_PASSWORD => "SELECT password FROM users WHERE username = ?"
        );

        private $db;
        private $queries;

        private static $airplane;

        /**
         * __construct()
         * Creates a new database object and checks for connection errors.
         */
        private function __construct() {
            // Connect to the database
            $this->db = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
        
            // Check connect errors
            if ($this->db->connect_error)
                die("Database connect error (" . $this->db->connect_errno . ") " . $this->db->connect_error);

            // Set autocommit mode
            $this->db->autocommit(false);
        }

        /**
         * __destruct()
         * Closes the database connection.
         */
        public function __destruct() {
            // Close all queries
            foreach ($this->queries as $stmt)
                $stmt->close();

            // Close database
            $this->db->close();
        }

        public static function get() {
            if (!isset(Airplane::$airplane))
                Airplane::$airplane = new Airplane();
            return Airplane::$airplane;
        }

        private function getOrPrepareQuery($stmt_num) {
            if (!isset($this->queries[$stmt_num]))
                $this->queries[$stmt_num] = $this->db->prepare(Airplane::STMT_SQL_CODE[$stmt_num]);
            return $this->queries[$stmt_num];
        }

        /**
         * getSeatStatus()
         * Returns the status for the given seat.
         */
        public function getSeatStatus($seat_num) {
            // Check seat correctness
            if (!SeatMap::checkSeatNum($seat_num))
                return new Message(false, "Invalid seat number: $seat_num.", null);

            // Prepare the query
            $stmt = $this->getOrPrepareQuery(Airplane::STMT_GET_SEAT);
            $stmt->bind_param("s", $seat_num);
            
            // Execute the query and check results
            $res = $stmt->execute();
            if (!$res)
                return new Message(false, "Database error: " . $stmt->errno. ".", null);

            // Read the status and return the seat object
            $stmt->bind_result($status, $reserver);
            $stmt->fetch();

            if (!isset($status) || !isset($reserver)) {
                $status = Seat::FREE;
                $reserver = null;
            }

            // Free the statements results
            $stmt->free_result();
    
            return new Message(true, null, new Seat($seat_num, $status, $reserver));
        }

        /**
         * getSeatStatusForUpdate()
         * Returns the status for the given seat (version using locks).
         */
        public function getSeatStatusForUpdate($seat_num) {
            // Check seat correctness
            if (!SeatMap::checkSeatNum($seat_num))
                return new Message(false, "Invalid seat number: $seat_num.", null);
            
            // Prepare the query
            $stmt = $this->getOrPrepareQuery(Airplane::STMT_GET_SEAT_FOR_UPDATE);
            $stmt->bind_param("s", $seat_num);
            
            // Execute the query and check results
            $res = $stmt->execute();
            if (!$res)
                return new Message(false, "Database error: " . $stmt->errno . ".", null);

            // Read the status and return the seat object
            $stmt->bind_result($status, $reserver);
            $stmt->fetch();

            if (!isset($status) || !isset($reserver)) {
                $status = Seat::FREE;
                $reserver = null;
            }

            $stmt->free_result();

            return new Message(true, null, new Seat($seat_num, $status, $reserver));
        }
    
        /**
         * getSeatStatusAll()
         * Returns the status of all seats in the database.
         */
        public function getSeatStatusAll() {
            $seatmap = new SeatMap();
    
            // Run the query on the DB
            $stmt = $this->getOrPrepareQuery(Airplane::STMT_GET_SEAT_ALL);
            $res = $stmt->execute();
            if (!$res)
                return new Message(false, "Database error: " . $stmt->errno . ".", null);
    
            // Loop through the results and fill the seat map
            $stmt->bind_result($seat_num, $status, $reserver);
            while ($stmt->fetch())
                $seatmap->updateSeat(new Seat($seat_num, $status, $reserver));
            
            // Free the statements results
            $stmt->free_result();
    
            return new Message(true, null, $seatmap);
        }

        /**
         * requestSeat()
         * Requests a seat in the airplane.
         */
        public function requestSeat($seat_num) {
            // Check that the user has logged in
            $session = Session::get(true);
            if ($session->getStatus() != Session::STATUS_OK)
                return new Message(false, "User session is not valid.", null);
            
            $user = $session->getUsername();
            $status = Seat::RESERVED;

            // Check seat correctness
            if (!SeatMap::checkSeatNum($seat_num))
                return new Message(false, "Invalid seat number: $seat_num.", null);

            // Begin the transaction
            $this->db->begin_transaction();
            
            // Check the current seat status
            $seat = $this->getSeatStatusForUpdate($seat_num);
            if ($seat->getSuccess() == false) {
                $this->db->rollback();
                return $seat;
            } else if ($seat->getData()->getStatus() === Seat::PURCHASED) {
                $this->db->rollback();
                return new Message(false, "Seat already purchased: $seat_num.", $seat->getData());
            }
    
            // Prepare the query
            if ($seat->getData()->getStatus() === Seat::SELECTED) {
                // Seat selected by the user, delete reservation
                $stmt = $this->getOrPrepareQuery(Airplane::STMT_DEL_RESERVATION);
                $stmt->bind_param("s", $seat_num);
                $msg = "Seat freed: $seat_num.";
            } else if ($seat->getData()->getStatus() === Seat::RESERVED) {
                // Seat selected by another user, update reservation
                $stmt = $this->getOrPrepareQuery(Airplane::STMT_UPD_RESERVATION);
                $stmt->bind_param("iss", $status, $user, $seat_num);
                $msg = "Seat selected from another user: $seat_num.";
            } else {
                // Seat free, insert reservation
                $stmt = $this->getOrPrepareQuery(Airplane::STMT_ADD_RESERVATION);
                $stmt->bind_param("sis", $seat_num, $status, $user);
                $msg = "Seat selected: $seat_num.";
            }
            
            // Execute the query and check results
            $res = $stmt->execute();
            if (!$res) {
                $this->db->rollback();
                return new Message(false, "Database error: " . $stmt->errno . ".", null);
            }

            // Free the statements results
            $stmt->free_result();

            // Commit the transaction
            $this->db->commit();
    
            return new Message(true, $msg, $this->getSeatStatus($seat_num)->getData());
        }

        /**
         * purchaseSeats()
         * Purchases the selected seats.
         */
        public function purchaseSeats($seats) {
            // Check that the user has logged in
            $session = Session::get(true);
            if ($session->getStatus() != Session::STATUS_OK)
                return new Message(false, "User session is not valid.", null);
            
            $user = $session->getUsername();
            $status = Seat::PURCHASED;
            
            // If no seat is requested, immediately return
            if (empty($seats))
                return $this->getSeatStatusAll();

            // Begin the transaction
            $this->db->begin_transaction();

            $add_seats = array();
            $upd_seats = array();
            $invalid_seats = array();

            // Check status of all places
            foreach ($seats as $seat_num) {
                $seat = $this->getSeatStatusForUpdate($seat_num);
                    
                if ($seat->getData()->getStatus() === Seat::FREE)
                    $add_seats[] = $seat->getData()->getSeatNum();
                else if ($seat->getData()->getStatus() === Seat::SELECTED)
                    $upd_seats[] = $seat->getData()->getSeatNum();
                else
                    $invalid_seats[] = $seat->getData()->getSeatNum();
            }

            if (empty($invalid_seats)) {
                // Prepare queries
                $stmt_add = $this->getOrPrepareQuery(Airplane::STMT_ADD_RESERVATION);
                $stmt_upd = $this->getOrPrepareQuery(Airplane::STMT_UPD_RESERVATION);
                
                foreach ($add_seats as $seat_num) {
                    $stmt_add->bind_param("sis", $seat_num, $status, $user);
                    
                    $res = $stmt_add->execute();
                    if (!$res) {
                        $this->db->rollback();
                        return new Message(false, "Database error: " . $stmt_add->errno . ".", null);
                    }
                }

                // Free the statements results
                $stmt_add->free_result();

                foreach ($upd_seats as $seat_num) {
                    $stmt_upd->bind_param("iss", $status, $user, $seat_num);
                    
                    $res = $stmt_upd->execute();
                    if (!$res) {
                        $this->db->rollback();
                        return new Message(false, "Database error: " . $stmt_upd->errno . ".", null);
                    }
                }

                // Free the statements results
                $stmt_upd->free_result();
            } else {
                // Free all places
                $stmt_del = $this->getOrPrepareQuery(Airplane::STMT_DEL_RESERVATION);

                foreach ($upd_seats as $seat_num) {
                    $stmt_del->bind_param("s", $seat_num);
                    
                    $res = $stmt_del->execute();
                    if (!$res) {
                        $this->db->rollback();
                        return new Message(false, "Database error: " . $stmt_del->errno . ".", null);
                    }
                }

                // Free the statements results
                $stmt_del->free_result();
            }

            // Commit the transaction
            $this->db->commit();

            if (empty($invalid_seats))
                return new Message(true, "Seats purchased: " . implode(", ", $seats) . ".", $this->getSeatStatusAll()->getData());
            
            return new Message(false, "Invalid seats: " . implode(", ", $invalid_seats) . ".", $this->getSeatStatusAll()->getData());
        }

        /**
         * createUser()
         * Creates a new user for the system.
         */
        public function createUser($username, $password) {
            // Check username is a valid email
            if (!preg_match("/^[a-zA-Z0-9\.\!\#\$\%\&\'\*\+\/\=\?\^\_\`\{\|\}\~\-]+@[a-zA-z0-9\-]+\.[a-zA-z0-9]+$/", $username))
                return new Message(false, "Registration failed: invalid username provided.", null);
            
            // Check password format
            if (!preg_match("/[a-z]/", $password) || (!preg_match("/[0-9A-Z]/", $password)))
                return new Message(false, "Registration failed: invalid password provided.", null);
            
            // Encode the password
            $enc_passw = password_hash($password, PASSWORD_DEFAULT);
            
            // Begin the transaction
            $this->db->begin_transaction();

            // Prepare the query
            $stmt = $this->getOrPrepareQuery(Airplane::STMT_ADD_USER);
            $stmt->bind_param("ss", $username, $enc_passw);

            // Execute the query and check results
            $res = $stmt->execute();
            if (!$res) {
                $this->db->rollback();
                
                // Check if it is a primary key constraint error
                if ($stmt->errno === 1062)
                    return new Message(false, "Registration failed: a user with the same name already exists.", null);
                else
                    return new Message(false, "Registration failed: database error ($stmt->errno).", null);
            }

            // Free the statements results
            $stmt->free_result();

            // Commit the transaction
            $this->db->commit();

            return new Message(true, "Registration successful.", null);
        }

        /**
         * checkUser()
         * Checks the login information of an user.
         */
        public function checkUser($username, $password) {
            // Prepare the query
            $stmt = $this->getOrPrepareQuery(Airplane::STMT_CHECK_PASSWORD);
            $stmt->bind_param("s", $username);

            // Execute the query and check results
            $res = $stmt->execute();
            if (!$res)
                return new Message(false, "Login failed: database error ($stmt->errno).", null);

            // Read the encoded password
            $stmt->bind_result($enc_passw);
            $stmt->fetch();

            // Free the statements results
            $stmt->free_result();
            
            // Check the password
            if (!isset($enc_passw) || !password_verify($password, $enc_passw))
                return new Message(false, "Login failed: incorrect username or password.", null);
            return new Message(true, "Login successful.", null);
        }
    }
?>