<?php
    /*
        get_seat_status.php
        API script for retrieving the status of a seat
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
    
    if (isset($_POST["seat"])) {
        // Create a database connection and get seat status
        $db = new Airplane();
        echo json_encode($db->getSeatStatus($_POST["seat"]));
    } else {
        // Create a database connection and get status of all seats
        $db = new Airplane();
        echo json_encode($db->getSeatStatusAll());
    }
?>