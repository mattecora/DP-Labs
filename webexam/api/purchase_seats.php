<?php
    /*
        purchase_seats.php
        API script for purchasing seats
        Matteo Corain - Distributed programming I - A.Y. 2018-19
    */

    require_once "../app/airplane.php";
    require_once "../app/session.php";

    enforce_https();
    header("Content-Type: application/json");

    // Check user login
    if (session_start_timeout() != SESSION_OK) {
        // User is unauthorized
        http_response_code(401);
        exit;
    }
    
    if (isset($_POST["seats"])) {
        // Create a database connection and perform purchase
        $db = new Airplane();
        echo json_encode($db->purchaseSeats($_POST["seats"]));
    } else {
        // Otherwise, bad request
        http_response_code(400);
    }
?>