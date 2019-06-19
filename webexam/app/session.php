<?php
    /*
        session.php
        Provides the Session class, for managing the user session
        Matteo Corain - Distributed programming I - A.Y. 2018-19
    */

    class Session {
        const TIMEOUT = 120;
        
        const STATUS_OK = 0;
        const STATUS_EXPIRED = 1;
        const STATUS_NONE = 2;

        private $status;

        /**
         * __construct()
         * Starts the session, checks the time difference and sets the status
         */
        public function __construct() {
            // Start the session
            if (session_status() === PHP_SESSION_NONE)
                session_start();

            // User has not logged in
            if (!isset($_SESSION["username"]) || !isset($_SESSION["last"])) {
                $this->status = Session::STATUS_NONE;
                return;
            }

            // Compute time difference
            $delta = time() - $_SESSION["last"];

            // Check time difference
            if ($delta > Session::TIMEOUT) {
                $this->logout();
                $this->status = Session::STATUS_EXPIRED;
                return;
            }
            
            // Everything ok, update time
            $_SESSION["last"] = time();
            $this->status = Session::STATUS_OK;
        }

        /**
         * getStatus()
         * Returns the status of the session
         */
        public function getStatus() {
            return $this->status;
        }

        /**
         * getUsername()
         * Returns the username of the logged user
         */
        public function getUsername() {
            return $_SESSION["username"];
        }

        /**
         * getLastAccess()
         * Returns the last access time of the logged user
         */
        public function getLastAccess() {
            return $_SESSION["last"];
        }

        /**
         * login()
         * Initializes the session variables, called after a successful login
         */
        public function login($username) {
            // Set session fields
            $_SESSION["username"] = $username;
            $_SESSION["last"] = time();

            // Set the session status
            $this->status = Session::STATUS_OK;
        }

        /**
         * logout()
         * Cleans up the session, called at logout time
         */
        public function logout() {
            // Unset the session variables
            $_SESSION = array();

            // If we are using cookies, delete the session cookie
            if (ini_get("session.use_cookies")) {
                $params = session_get_cookie_params();
                setcookie(session_name(), '', time() - 3600,
                    $params["path"], $params["domain"],
                    $params["secure"], $params["httponly"]
                );
            }

            // Destroy the session
            session_destroy();

            // Set the session status
            $this->status = Session::STATUS_NONE;
        }
    }
?>