<?php
    /*
        session.php
        Provides the functions for managing the user session
        Matteo Corain - Distributed programming I - A.Y. 2018-19
    */

    define("TIMEOUT", 120);
    
    define("SESSION_OK", 0);
    define("SESSION_EXPIRED", 1);
    define("NO_SESSION", 2);

    /**
     * enforce_https()
     * Simple function to redirect on the HTTPS version of the requested page
     */
    function enforce_https() {
        if (!isset($_SERVER["HTTPS"]))
            header("Location: https://" . $_SERVER["HTTP_HOST"] . $_SERVER["REQUEST_URI"]);
    }

    /**
     * user_is_logged()
     * Function to check if the user has logged without starting the session again
     */
    function user_is_logged() {
        return isset($_SESSION["username"]);
    }

    /**
     * session_start_login()
     * Function to initialize the session variables, called after a successful login
     */
    function session_start_login($username) {
        // Start the session
        session_start();

        // Set session fields
        $_SESSION["username"] = $username;
        $_SESSION["last"] = time();
    }

    /**
     * session_start_timeout()
     * Function to start the session and check the time difference with the last access
     */
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

    /**
     * session_logout()
     * Function to cleanup the session, called at logout time
     */
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