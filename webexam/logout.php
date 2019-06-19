<!--
    logout.php
    Logout page of the website
    Matteo Corain - Distributed programming I - A.Y. 2018-19
-->

<?php
    require_once "app/https.php";
    require_once "app/session.php";

    enforce_https();

    $session = new Session();

    // If session is still valid, logout
    if ($session->getStatus() === Session::STATUS_OK)
        $session->logout();

    // Go back to the home page
    header("Location: index.php");
?>