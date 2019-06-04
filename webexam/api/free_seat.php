<?php
    require_once("../app/enforce_https.php");
    require_once("../app/db.php");
    require_once("../app/session.php");

    header("Content-Type: application/json");

    // Check user login
    if (session_start_timeout() != SESSION_OK) {
        http_response_code(403);
        die(json_encode(array("error" => "User is not logged in")));
    }

    if (isset($_POST["seat"])) {
        // Create a database connection and request a seat
        $db = new AirplaneDatabase();
        $seat = $db->freeSeat($_POST["seat"]);

        // Return the JSON-encoded seat
        echo json_encode($seat);
    }
?>