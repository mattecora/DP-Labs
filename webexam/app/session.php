<?php
    /*
        session.php
        Provides the functions for managing the user session
        Matteo Corain - Distributed programming I - A.Y. 2018-19
    */

    const TIMEOUT = 120;
    
    const SESSION_OK = 0;
    const SESSION_EXPIRED = 1;
    const NO_SESSION = 2;

    function user_is_logged() {
        return isset($_SESSION["username"]);
    }

    function session_start_login($username) {
        // Start the session
        session_start();

        // Set session fields
        $_SESSION["username"] = $username;
        $_SESSION["last"] = time();
    }

    function session_start_timeout() {
        // Start the session
        if (session_status() === PHP_SESSION_NONE)
            session_start();

        // User has not logged in
        if (!isset($_SESSION["username"]) || !isset($_SESSION["last"])) {
            return NO_SESSION;
        }

        // Compute time difference
        $delta = time() - $_SESSION["last"];

        // Check time difference
        if ($delta > TIMEOUT) {
            session_logout();
            return SESSION_EXPIRED;
        }
        
        // Everything ok, update time
        $_SESSION["last"] = time();
        return SESSION_OK;
    }

    function session_logout() {
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
    }
?>