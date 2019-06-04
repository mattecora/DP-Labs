<?php
    require_once("app/session.php");

    // If session is still valid, logout
    if (session_start_timeout() == SESSION_OK)
        session_logout();

    // Go back to the home page
    header("Location: index.php");
?>