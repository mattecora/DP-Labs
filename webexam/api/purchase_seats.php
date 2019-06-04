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
    
    if (isset($_POST["seats"])) {
        // Create a database connection and perform purchase
        $db = new AirplaneDatabase();
        $seatmap = $db->purchaseSeats($_POST["seats"]);

        // Return the JSON-encoded seat map
        echo json_encode($seatmap);
    } else {
        // Otherwise, bad request
        http_response_code(400);
        die(json_encode(array("error" => "Invalid data format")));
    }
?>