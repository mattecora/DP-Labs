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

        /**
         * get()
         * Get the singleton instance of the class.
         */
        public static function get() {
            if (!isset(Airplane::$airplane))
                Airplane::$airplane = new Airplane();
            return Airplane::$airplane;
        }

        /**
         * getOrPrepareQuery()
         * Prepare a statement or return it if already available.
         */
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
                return new Message(false, "Seat retrieval failed: invalid seat number ($seat_num).", null);

            // Prepare the query and bind parameters
            $stmt = $this->getOrPrepareQuery(Airplane::STMT_GET_SEAT);
            if (!$stmt || !$stmt->bind_param("s", $seat_num))
                return new Message(false, "Seat retrieval failed: database error ($stmt->errno).", null);
            
            // Execute the query and bind results
            if (!$stmt->execute() || !$stmt->bind_result($status, $reserver))
                return new Message(false, "Seat retrieval failed: database error ($stmt->errno).", null);

            // Check if seat is in database, otherwise it is free
            if (!$stmt->fetch()) {
                $status = Seat::FREE;
                $reserver = null;
            }

            // Free the statements results
            $stmt->free_result();
    
            return new Message(true, "Seat retrieval successful: $seat_num.", new Seat($seat_num, $status, $reserver));
        }

        /**
         * getSeatStatusForUpdate()
         * Returns the status for the given seat (version using locks).
         */
        public function getSeatStatusForUpdate($seat_num) {
            // Check seat correctness
            if (!SeatMap::checkSeatNum($seat_num))
                return new Message(false, "Seat retrieval failed: invalid seat number ($seat_num).", null);
            
            // Prepare the query
            $stmt = $this->getOrPrepareQuery(Airplane::STMT_GET_SEAT_FOR_UPDATE);
            if (!$stmt || !$stmt->bind_param("s", $seat_num))
                return new Message(false, "Seat retrieval failed: database error ($stmt->errno).", null);
            
            // Execute the query and bind results
            if (!$stmt->execute() || !$stmt->bind_result($status, $reserver))
                return new Message(false, "Seat retrieval failed: database error ($stmt->errno).", null);

            // Check if seat is in database, otherwise it is free
            if (!$stmt->fetch()) {
                $status = Seat::FREE;
                $reserver = null;
            }

            $stmt->free_result();

            return new Message(true, "Seat retrieval successful: $seat_num.", new Seat($seat_num, $status, $reserver));
        }
    
        /**
         * getSeatStatusAll()
         * Returns the status of all seats in the database.
         */
        public function getSeatStatusAll() {
            $seatmap = new SeatMap();
    
            // Run the query on the DB
            $stmt = $this->getOrPrepareQuery(Airplane::STMT_GET_SEAT_ALL);
            if (!$stmt || !$stmt->execute())
                return new Message(false, "Seat retrieval failed: database error ($stmt->errno).", null);
    
            // Bind results
            if (!$stmt->bind_result($seat_num, $status, $reserver))
                return new Message(false, "Seat retrieval failed: database error ($stmt->errno).", null);
            
            // Fetch all results and fill the seat map
            while ($stmt->fetch())
                $seatmap->updateSeat(new Seat($seat_num, $status, $reserver));
            
            // Free the statements results
            $stmt->free_result();
    
            return new Message(true, "Seat retrieval successful.", $seatmap);
        }

        /**
         * requestSeat()
         * Requests a seat in the airplane.
         */
        public function requestSeat($seat_num) {
            // Check that the user has logged in
            $session = Session::get(true);
            if ($session->getStatus() != Session::STATUS_OK)
                return new Message(false, "Seat selection failed: user session is not valid.", null);
            
            $user = $session->getUsername();
            $status = Seat::RESERVED;

            // Begin the transaction
            $this->db->begin_transaction();
            
            // Check the current seat status
            $seat = $this->getSeatStatusForUpdate($seat_num);
            if (!$seat->getSuccess()) {
                $this->db->rollback();
                return $seat;
            } else if ($seat->getData()->getStatus() === Seat::PURCHASED) {
                $this->db->rollback();
                return new Message(false, "Seat selection failed: $seat_num already purchased.", $seat->getData());
            }
    
            // Prepare the query
            if ($seat->getData()->getStatus() === Seat::RESERVED) {
                // Seat selected by another user, update reservation
                $stmt = $this->getOrPrepareQuery(Airplane::STMT_UPD_RESERVATION);

                if (!$stmt || !$stmt->bind_param("iss", $status, $user, $seat_num)) {
                    $this->db->rollback();
                    return new Message(false, "Seat selection failed: database error ($stmt->errno).", null);
                }
                
                $msg = "Seat selection successful: $seat_num selected from another user.";
            } else if ($seat->getData()->getStatus() === Seat::FREE) {
                // Seat free, insert reservation
                $stmt = $this->getOrPrepareQuery(Airplane::STMT_ADD_RESERVATION);

                if (!$stmt || !$stmt->bind_param("sis", $seat_num, $status, $user)) {
                    $this->db->rollback();
                    return new Message(false, "Seat selection failed: database error ($stmt->errno).", null);
                }

                $msg = "Seat selection successful: $seat_num selected.";
            }
            
            // Execute the query and check results
            if (isset($stmt) && !$stmt->execute()) {
                $this->db->rollback();
                return new Message(false, "Seat selection failed: database error ($stmt->errno).", null);
            }

            // Free the statements results
            $stmt->free_result();

            // Commit the transaction
            $this->db->commit();
    
            return new Message(true, $msg, $this->getSeatStatus($seat_num)->getData());
        }

        /**
         * freeSeat()
         * Free a seat in the airplane.
         */
        public function freeSeat($seat_num) {
            // Check that the user has logged in
            $session = Session::get(true);
            if ($session->getStatus() != Session::STATUS_OK)
                return new Message(false, "Seat release failed: user session is not valid.", null);
            
            $user = $session->getUsername();
            $status = Seat::FREE;

            // Begin the transaction
            $this->db->begin_transaction();
            
            // Check the current seat status
            $seat = $this->getSeatStatusForUpdate($seat_num);
            if (!$seat->getSuccess()) {
                $this->db->rollback();
                return $seat;
            } else if ($seat->getData()->getStatus() === Seat::PURCHASED) {
                $this->db->rollback();
                return new Message(false, "Seat release failed: $seat_num already purchased.", $seat->getData());
            } else if ($seat->getData()->getStatus() === Seat::RESERVED) {
                $this->db->rollback();
                return new Message(false, "Seat release failed: $seat_num is reserved by someone else.", $seat->getData());
            }
    
            // Prepare the query
            if ($seat->getData()->getStatus() === Seat::SELECTED) {
                // Seat selected by the user, delete reservation
                $stmt = $this->getOrPrepareQuery(Airplane::STMT_DEL_RESERVATION);

                if (!$stmt || !$stmt->bind_param("s", $seat_num)) {
                    $this->db->rollback();
                    return new Message(false, "Seat release failed: database error ($stmt->errno).", null);
                }

                // Execute the query and check results
                if (!$stmt->execute()) {
                    $this->db->rollback();
                    return new Message(false, "Seat release failed: database error ($stmt->errno).", null);
                }

                // Free the statements results
                $stmt->free_result();
            }

            // Commit the transaction
            $this->db->commit();
    
            return new Message(true, "Seat release successful: $seat_num freed.", $this->getSeatStatus($seat_num)->getData());
        }

        /**
         * purchaseSeats()
         * Purchases the selected seats.
         */
        public function purchaseSeats($seats) {
            // Check that the user has logged in
            $session = Session::get(true);
            if ($session->getStatus() != Session::STATUS_OK)
                return new Message(false, "Seats purchase failed: user session is not valid.", null);
            
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
                    if (!$stmt_add || !$stmt_add->bind_param("sis", $seat_num, $status, $user) || !$stmt_add->execute()) {
                        $this->db->rollback();
                        return new Message(false, "Seats purchase failed: database error ($stmt_add->errno).", null);
                    }
                }

                // Free the statements results
                $stmt_add->free_result();

                foreach ($upd_seats as $seat_num) {
                    if (!$stmt_upd || !$stmt_upd->bind_param("iss", $status, $user, $seat_num) || !$stmt_upd->execute()) {
                        $this->db->rollback();
                        return new Message(false, "Seats purchase failed: database error ($stmt_upd->errno).", null);
                    }
                }

                // Free the statements results
                $stmt_upd->free_result();
            } else {
                // Free all places
                $stmt_del = $this->getOrPrepareQuery(Airplane::STMT_DEL_RESERVATION);
                if (!$stmt_del) {
                    $this->db->rollback();
                    return new Message(false, "Seats purchase failed: database error ($stmt_del->errno).", null);
                }

                foreach ($upd_seats as $seat_num) {
                    if (!$stmt_del->bind_param("s", $seat_num) || !$stmt_del->execute()) {
                        $this->db->rollback();
                        return new Message(false, "Seats purchase failed: database error ($stmt_del->errno).", null);
                    }
                }

                // Free the statements results
                $stmt_del->free_result();
            }

            // Commit the transaction
            $this->db->commit();

            if (empty($invalid_seats))
                return new Message(true, "Seats purchase successful: " . implode(", ", $seats) . " purchased.", $this->getSeatStatusAll()->getData());
            
            return new Message(false, "Seat purchase failed: invalid seats (" . implode(", ", $invalid_seats) . ").", $this->getSeatStatusAll()->getData());
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
            if (!$stmt || !$stmt->bind_param("ss", $username, $enc_passw)) {
                $this->db->rollback();
                return new Message(false, "Registration failed: database error ($stmt->errno).", null);
            }

            // Execute the query and check results
            if (!$stmt->execute()) {
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
            if (!$stmt || !$stmt->bind_param("s", $username))
                return new Message(false, "Login failed: database error ($stmt->errno).", null);

            // Execute the query and bind results
            if (!$stmt->execute() || !$stmt->bind_result($enc_passw))
                return new Message(false, "Login failed: database error ($stmt->errno).", null);

            // Check the password
            if (!$stmt->fetch() || !password_verify($password, $enc_passw))
                return new Message(false, "Login failed: incorrect username or password.", null);

            // Free the statements results
            $stmt->free_result();
            
            // Login ok
            return new Message(true, "Login successful.", null);
        }
    }
?>