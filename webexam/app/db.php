<?php
    require_once("globals.php");
    require_once("seat.php");
    require_once("seatmap.php");
    require_once("session.php");

    class AirplaneDatabase {
        private const HOST = "localhost";
        private const USER = "root";
        private const PASS = "";
        private const DBNM = "dpexam";

        private $db;

        /**
         * Constructor of the database.
         * Creates a new mysqli object and checks for connection errors.
         */
        public function __construct() {
            $this->db = new mysqli(AirplaneDatabase::HOST, AirplaneDatabase::USER, AirplaneDatabase::PASS, AirplaneDatabase::DBNM);
        
            if ($this->db->connect_error) {
                die("Database connect error (" . $this->db->connect_errno . ") " . $this->db->connect_error);
            }
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
            // Prepare the query
            $stmt = $this->db->prepare("SELECT status, reserver FROM airplane WHERE seat = ?");
            $stmt->bind_param("s", $seat_num);
            
            // Execute the query and check results
            $res = $stmt->execute();
            if (!$res) {
                //die("Database error (" . $this->db->errno . ") " . $this->db->error);
                return false;
            }

            // Read the status and return the seat object
            $stmt->bind_result($status, $reserver);
            $stmt->fetch();
            $seat = new Seat($seat_num, $status, $reserver);
    
            return $seat;
        }

        /**
         * Returns the status for the given seat (version using locks).
         */
        public function getSeatStatusForUpdate($seat_num) {
            // Prepare the query
            $stmt = $this->db->prepare("SELECT status, reserver FROM airplane WHERE seat = ? FOR UPDATE");
            $stmt->bind_param("s", $seat_num);
            
            // Execute the query and check results
            $res = $stmt->execute();
            if (!$res) {
                //die("Database error (" . $this->db->errno . ") " . $this->db->error);
                return false;
            }

            // Read the status and return the seat object
            $stmt->bind_result($status, $reserver);
            $stmt->fetch();
            $seat = new Seat($seat_num, $status, $reserver);
    
            return $seat;
        }
    
        /**
         * Return the status of all seats in the database.
         */
        public function getSeatStatusAll() {
            $seatmap = new SeatMap();
    
            // Run the query on the DB
            $stmt = $this->db->prepare("SELECT * FROM airplane");
            $res = $stmt->execute();
            if (!$res) {
                //die("Database error (" . $this->db->errno . ") " . $this->db->error);
                return false;
            }
    
            // Loop through the results and fill the seat map
            $stmt->bind_result($seat_num, $status, $reserver);
            while ($stmt->fetch()) {
                $seatmap->updateSeat(new Seat($seat_num, $status, $reserver));
            }
    
            return $seatmap;
        }

        /**
         * Request a seat in the airplane.
         */
        public function requestSeat($seat_num) {
            // Check that the user has logged in
            session_start_timeout();
            if (!user_is_logged())
                die("User is not logged in");

            // Begin the transaction
            $this->db->begin_transaction();
            
            // Check the current seat status
            $seat = $this->getSeatStatusForUpdate($seat_num);
            if ($seat == false) {
                $this->db->rollback();
                return false;
            } else if ($seat->getStatus() === Seat::INVALID || $seat->getStatus() === Seat::PURCHASED) {
                $this->db->rollback();
                return $seat;
            }
    
            // Prepare the query
            $stmt = $this->db->prepare("UPDATE airplane SET status = 1, reserver = ? WHERE seat = ?");
            $stmt->bind_param("ss", $_SESSION["username"], $seat_num);
            
            // Execute the query and check results
            $res = $stmt->execute();
            if (!$res) {
                $this->db->rollback();
                //die("Database error (" . $this->db->errno . ") " . $this->db->error);
                return false;
            }

            // Commit the transaction
            $this->db->commit();
    
            return $this->getSeatStatus($seat_num);
        }

        /**
         * Frees a selected seat in the airplane.
         */
        public function freeSeat($seat_num) {
            // Check that the user has logged in
            session_start_timeout();
            if (!user_is_logged())
                die("User is not logged in");

            // Begin the transaction
            $this->db->begin_transaction();
            
            // Check the current seat status
            $seat = $this->getSeatStatusForUpdate($seat_num);
            if ($seat == false) {
                $this->db->rollback();
                return false;
            } else if ($seat->getStatus() === Seat::INVALID || $seat->getStatus() === Seat::PURCHASED || $seat->getReserver() !== $_SESSION["username"]) {
                $this->db->rollback();
                return $seat;
            }
    
            // Prepare the query
            $stmt = $this->db->prepare("UPDATE airplane SET status = 0, reserver = null WHERE seat = ?");
            $stmt->bind_param("s", $seat_num);
            
            // Execute the query and check results
            $res = $stmt->execute();
            if (!$res) {
                $this->db->rollback();
                //die("Database error (" . $this->db->errno . ") " . $this->db->error);
                return false;
            }

            // Commit the transaction
            $this->db->commit();
    
            return $this->getSeatStatus($seat_num);
        }

        /**
         * Purchases the selected seats.
         */
        public function purchaseSeats($seats) {
            // Check that the user has logged in
            session_start_timeout();
            if (!user_is_logged())
                die("User is not logged in");
            
            // If no seat is requested, immediately return
            if (empty($seats))
                return true;

            // Begin the transaction
            $this->db->begin_transaction();

            $valid = true;

            // Check status of all places
            foreach ($seats as $seat_num) {
                $seat = $this->getSeatStatusForUpdate($seat_num);
                if ($seat->getStatus() !== Seat::RESERVED || $seat->getReserver() !== $_SESSION["username"]) {
                    // At least a request is invalid
                    $valid = false;
                }
            }

            if ($valid) {
                // Set places as purchased
                $stmt = $this->db->prepare("UPDATE airplane SET status = 2 WHERE status = 1 AND reserver = ?");
                $stmt->bind_param("s", $_SESSION["username"]);
                $stmt->execute();
            } else {
                // Free all places
                $stmt = $this->db->prepare("UPDATE airplane SET status = 0, reserver= '' WHERE status = 1 AND reserver = ?");
                $stmt->bind_param("s", $_SESSION["username"]);
                $stmt->execute();
            }

            // Commit the transaction
            $this->db->commit();
    
            return $this->getSeatStatusAll();
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
                //die("Database error (" . $this->db->errno . ") " . $this->db->error);
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
                //die("Database error (" . $this->db->errno . ") " . $this->db->error);
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