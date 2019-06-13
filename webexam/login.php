<!--
    login.php
    Login page of the website
    Matteo Corain - Distributed programming I - A.Y. 2018-19
-->

<?php
    require_once "app/airplane.php";
    require_once "app/session.php" ;

    $session = new Session();

    if (isset($_POST["username"]) && isset($_POST["password"])) {
        // Open the DB and add new user
        $airplane = new Airplane();

        // Verify login info
        $msg = $airplane->checkUser($_POST["username"], $_POST["password"]);

        if ($msg->getSuccess()) {
            // Login the user
            $session->login($_POST["username"]);

            // Redirect to the index page
            header("Location: index.php");
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
        <?php require_once "components/header.php"; ?>
    </div>

    <div id="main" class="container hidden">
        <div class="row">
            <?php require_once "components/menu.php"; ?>

            <div class="contents">
                <h2>Login</h2>
                <form action="login.php" method="post">
                    <input type="email" name="username" id="username" placeholder="Username">
                    <input type="password" name="password" id="password" placeholder="Password">
                    <input type="submit" value="Login" class="button">
                </form>
            </div>
        </div>
    </div>

    <?php require_once "components/jscheck.php"; ?>

    <script src="js/jquery-3.4.1.min.js"></script>
    <script src="js/login.js"></script>
    <script>
        $(document).ready(function() {
            <?php if (!$msg->getSuccess()) { ?>
                alert("<?= $msg->getMessage() ?>");
            <?php } ?>
        });
    </script>
</body>
</html>