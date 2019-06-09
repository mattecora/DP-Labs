<?php
    require_once "../app/enforce_https.php";
    require_once "../app/db.php";
    require_once "../app/session.php";

    header("Content-Type: application/json");

    // Check user login
    if (session_start_timeout() != SESSION_OK) {
        // User is unauthorized
        http_response_code(401);
        exit;
    }

    if (isset($_POST["seat"])) {
        // Create a database connection and request a seat
        $db = new AirplaneDatabase();
        echo json_encode($db->requestSeat($_POST["seat"]));
    } else {
        // Otherwise, bad request
        http_response_code(400);
    }
?>