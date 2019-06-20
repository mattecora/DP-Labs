<?php
    /*
        get_seat_status.php
        API script for retrieving the status of a seat
        Matteo Corain - Distributed programming I - A.Y. 2018-19
    */

    require_once "../app/airplane.php";
    require_once "../app/session.php";

    $session = Session::get(true);
    header("Content-Type: application/json");

    // Check user login
    if ($session->getStatus() != Session::STATUS_OK) {
        // User is unauthorized
        http_response_code(401);
        exit;
    }
    
    if (isset($_POST["seat"])) {
        // Create a database connection and get seat status
        $airplane = Airplane::get();
        echo json_encode($airplane->getSeatStatus($_POST["seat"]));
    } else {
        // Create a database connection and get status of all seats
        $airplane = Airplane::get();
        echo json_encode($airplane->getSeatStatusAll());
    }
?>