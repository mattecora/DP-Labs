<?php
    /*
        enforce_https.php
        Simple script for enforcing HTTPS usage
        Matteo Corain - Distributed programming I - A.Y. 2018-19
    */
    
    if ($_SERVER["HTTPS"] != "on") {
        header("Location: https://" . $_SERVER["HTTP_HOST"] . $_SERVER["REQUEST_URI"]);
        exit;
    }
?>