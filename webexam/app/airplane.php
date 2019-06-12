<?php
    /*
        airplane.php
        Provides the facade class for the application, containing the DB interaction
        Matteo Corain - Distributed programming I - A.Y. 2018-19
    */

    require_once "message.php";
    require_once "seat.php";
    require_once "seatmap.php";
    require_once "session.php";

    class Airplane {
        private const HOST = "localhost";
        private const USER = "root";
        private const PASS = "";
        private const NAME = "dpexam";

        private $db;

        /**
         * Constructor of the database.
         * Creates a new mysqli object and checks for connection errors.
         */
        public function __construct() {
            $this->db = new mysqli(Airplane::HOST, Airplane::USER, Airplane::PASS, Airplane::NAME);
        
            if ($this->db->connect_error)
                die("Database connect error (" . $this->db->connect_errno . ") " . $this->db->connect_error);
        }

        /**
         * Destructor of the database.
         * Closes the database connection.
         */
        public function __destruct() {
            $this->db->close();
        }

        /**
         * Returns the status for the given seat.
         */
        public function getSeatStatus($seat_num) {
            // Check seat correctness
            if (!SeatMap::checkSeatNum($seat_num))
                return new Message(false, "Invalid seat number: $seat_num.", null);

            // Prepare the query
            $stmt = $this->db->prepare("SELECT status, reserver FROM airplane WHERE seat = ?");
            $stmt->bind_param("s", $seat_num);
            
            // Execute the query and check results
            $res = $stmt->execute();
            if (!$res)
                return new Message(false, "Database error: " . $this->db->errno. ".", null);

            // Read the status and return the seat object
            $stmt->bind_result($status, $reserver);
            $stmt->fetch();
    
            return new Message(true, null, new Seat($seat_num, $status, $reserver));
        }

        /**
         * Returns the status for the given seat (version using locks).
         */
        public function getSeatStatusForUpdate($seat_num) {
            // Check seat correctness
            if (!SeatMap::checkSeatNum($seat_num))
                return new Message(false, "Invalid seat number: $seat_num.", null);
            
            // Prepare the query
            $stmt = $this->db->prepare("SELECT status, reserver FROM airplane WHERE seat = ? FOR UPDATE");
            $stmt->bind_param("s", $seat_num);
            
            // Execute the query and check results
            $res = $stmt->execute();
            if (!$res)
                return new Message(false, "Database error: " . $this->db->errno . ".", null);

            // Read the status and return the seat object
            $stmt->bind_result($status, $reserver);
            $stmt->fetch();

            return new Message(true, null, new Seat($seat_num, $status, $reserver));
        }
    
        /**
         * Return the status of all seats in the database.
         */
        public function getSeatStatusAll() {
            $seatmap = new SeatMap();
    
            // Run the query on the DB
            $stmt = $this->db->prepare("SELECT * FROM airplane");
            $res = $stmt->execute();
            if (!$res)
                return new Message(false, "Database error: " . $this->db->errno . ".", null);
    
            // Loop through the results and fill the seat map
            $stmt->bind_result($seat_num, $status, $reserver);
            while ($stmt->fetch())
                $seatmap->updateSeat(new Seat($seat_num, $status, $reserver));
    
            return new Message(true, null, $seatmap);
        }

        /**
         * Request a seat in the airplane.
         */
        public function requestSeat($seat_num) {
            // Check that the user has logged in
            session_start_timeout();
            if (!user_is_logged())
                return new Message(false, "User is not logged in.", null);

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
                $stmt = $this->db->prepare("UPDATE airplane SET status = 0, reserver = NULL WHERE seat = ?");
                $stmt->bind_param("s", $seat_num);
                $msg = "Seat freed: $seat_num.";
            } else if ($seat->getData()->getStatus() === Seat::RESERVED) {
                // Seat selected by another user, update reservation
                $stmt = $this->db->prepare("UPDATE airplane SET reserver = ? WHERE seat = ?");
                $stmt->bind_param("ss", $_SESSION["username"], $seat_num);
                $msg = "Seat selected from another user: $seat_num.";
            } else {
                // Seat free, insert reservation
                $stmt = $this->db->prepare("UPDATE airplane SET status = 1, reserver = ? WHERE seat = ?");
                $stmt->bind_param("ss", $_SESSION["username"], $seat_num);
                $msg = "Seat selected: $seat_num.";
            }
            
            // Execute the query and check results
            $res = $stmt->execute();
            if (!$res) {
                $this->db->rollback();
                return new Message(false, "Database error: " . $this->db->error . ".", null);
            }

            // Commit the transaction
            $this->db->commit();
    
            return new Message(true, $msg, $this->getSeatStatus($seat_num)->getData());
        }

        /**
         * Purchases the selected seats.
         */
        public function purchaseSeats($seats) {
            // Check that the user has logged in
            session_start_timeout();
            if (!user_is_logged())
                return new Message(false, "User is not logged in.", null);
            
            // If no seat is requested, immediately return
            if (empty($seats))
                return $this->getSeatStatusAll();

            // Begin the transaction
            $this->db->begin_transaction();

            $invalid_seats = array();

            // Check status of all places
            foreach ($seats as $seat_num) {
                $seat = $this->getSeatStatusForUpdate($seat_num);
                if ($seat->getData()->getStatus() !== Seat::SELECTED)
                    $invalid_seats[] = $seat->getData()->getSeatNum();
            }

            if (empty($invalid_seats)) {
                // Set places as purchased
                $stmt = $this->db->prepare("UPDATE airplane SET status = 2 WHERE status = 1 AND reserver = ?");
                $stmt->bind_param("s", $_SESSION["username"]);
            } else {
                // Free all places
                $stmt = $this->db->prepare("UPDATE airplane SET status = 0, reserver = NULL WHERE status = 1 AND reserver = ?");
                $stmt->bind_param("s", $_SESSION["username"]);
            }

            // Commit the transaction
            $stmt->execute();
            $this->db->commit();

            if (empty($invalid_seats))
                return new Message(true, "Seats purchased: " . implode(", ", $seats) . ".", $this->getSeatStatusAll()->getData());
            
            return new Message(false, "Invalid seats: " . implode(", ", $invalid_seats) . ".", $this->getSeatStatusAll()->getData());
        }

        /**
         * Create a new user for the system.
         */
        public function createUser($username, $password) {
            // Check username is a valid email
            if (!preg_match("/^[a-zA-z0-9]+@[a-zA-z0-9]+\.[a-zA-z0-9]+$/", $username))
                return false;
            
            // Check password format
            if (!preg_match("/[a-z]/", $password) || (!preg_match("/[0-9A-Z]/", $password)))
                return false;
            
            // Encode the password
            $enc_passw = password_hash($password, PASSWORD_DEFAULT);
            
            // Begin the transaction
            $this->db->begin_transaction();

            // Prepare the query
            $stmt = $this->db->prepare("INSERT INTO users VALUES (?, ?)");
            $stmt->bind_param("ss", $username, $enc_passw);

            // Execute the query and check results
            $res = $stmt->execute();
            if (!$res) {
                $this->db->rollback();
                return false;
            }

            // Commit the transaction
            $this->db->commit();

            return true;
        }

        /**
         * Check the login information of an user.
         */
        public function checkUser($username, $password) {
            // Prepare the query
            $stmt = $this->db->prepare("SELECT password FROM users WHERE username = ?");
            $stmt->bind_param("s", $username);

            // Execute the query and check results
            $res = $stmt->execute();
            if (!$res) {
                $this->db->rollback();
                return false;
            }

            // Read the encoded password
            $stmt->bind_result($enc_passw);
            $stmt->fetch();
            
            // Check the password
            return password_verify($password, $enc_passw);
        }
    }
?>