<!--
    logout.php
    Logout page of the website
    Matteo Corain - Distributed programming I - A.Y. 2018-19
-->

<?php
    require_once "app/session.php";

    enforce_https();

    // If session is still valid, logout
    if (session_start_timeout() === SESSION_OK)
        session_logout();

    // Go back to the home page
    header("Location: index.php");
?>