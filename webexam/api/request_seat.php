<?php
    /*
        request_seat.php
        API script for requesting a seat
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
        // Create a database connection and request a seat
        $db = new Airplane();
        echo json_encode($db->requestSeat($_POST["seat"]));
    } else {
        // Otherwise, bad request
        http_response_code(400);
    }
?>