<?php
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
        if (session_status() == PHP_SESSION_NONE)
            session_start();

        // User has not logged in
        if (!isset($_SESSION["username"]) || !isset($_SESSION["last"])) {
            session_unset();
            return NO_SESSION;
        }

        // Compute time difference
        $delta = time() - $_SESSION["last"];

        // Check time difference
        if ($delta > TIMEOUT) {
            session_unset();
            session_destroy();
            return SESSION_EXPIRED;
        }
        
        // Everything ok, update time
        $_SESSION["last"] = time();
        return SESSION_OK;
    }

    function session_logout() {
        session_unset();
        session_destroy();
    }
?>