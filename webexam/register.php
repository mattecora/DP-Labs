<?php
    require_once("app/enforce_https.php");
    require_once("app/db.php");
    require_once("app/session.php");

    if (isset($_POST["username"]) && isset($_POST["password"])) {
        // Open the DB and add new user
        $db = new AirplaneDatabase();

        if ($db->createUser($_POST["username"], $_POST["password"])) {
            // Login the user
            session_start_login($_POST["username"]);

            // Redirect to the index page
            header("Location: index.php");
        } else {
            $creation_failed = true;
        }
    }
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Airplane seats reservation</title>

    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div class="container">
        <?php require_once("components/header.php"); ?>
    </div>

    <div class="container">
        <div class="row">
            <?php require_once("components/menu.php"); ?>

            <div class="contents">
                <h2>Register</h2>
                <form action="register.php" method="post" onsubmit="return checkRegistration();">
                    <input type="email" name="username" id="username" placeholder="Username">
                    <input type="password" name="password" id="password" placeholder="Password">
                    <input type="submit" value="Register">
                </form>
            </div>
        </div>
    </div>

    <?php if (isset($creation_failed)) echo "<script>alert(\"Cannot create user!\");</script>"; ?>

    <script src="js/jquery-3.4.1.min.js"></script>
    <script src="js/login.js"></script>
</body>
</html>