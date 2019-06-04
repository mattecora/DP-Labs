<?php
    require_once("../app/enforce_https.php");
    require_once("../app/db.php");
    require_once("../app/session.php");

    header("Content-Type: application/json");

    // Check user login
    if (session_start_timeout() != SESSION_OK) {
        // User is unauthorized
        http_response_code(401);
        die(json_encode(array("error" => "User is not logged in")));
    }
    
    if (isset($_POST["seat"])) {
        // Create a database connection and get seat status
        $db = new AirplaneDatabase();
        $seat = $db->getSeatStatus($_POST["seat"]);

        // Return the JSON-encoded seat
        echo json_encode($seat);
    } else {
        // Create a database connection and get seat status
        $db = new AirplaneDatabase();
        $seatmap = $db->getSeatStatusAll();

        // Return the JSON-encoded seat
        echo json_encode($seatmap);
    }
?>