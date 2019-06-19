<?php
    /*
        https.php
        Provides the function to enforce HTTPS usage
        Matteo Corain - Distributed programming I - A.Y. 2018-19
    */

    function enforce_https() {
        // Enforce HTTPS usage
        if (!isset($_SERVER["HTTPS"])) {
            header("Location: https://" . $_SERVER["HTTP_HOST"] . $_SERVER["REQUEST_URI"]);
            exit;
        }
    }
?>