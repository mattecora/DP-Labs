<?php
    /*
        request_seat.php
        API script for requesting a seat
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
        // Create a database connection and request a seat
        $airplane = Airplane::get();
        echo json_encode($airplane->requestSeat($_POST["seat"]));
    } else {
        // Otherwise, bad request
        http_response_code(400);
    }
?>