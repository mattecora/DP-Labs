<!--
    register.php
    Registration page of the website
    Matteo Corain - Distributed programming I - A.Y. 2018-19
-->

<?php
    require_once "app/airplane.php";
    require_once "app/https.php";
    require_once "app/session.php";

    enforce_https();

    // Open session
    $session = Session::get(false);

    if (isset($_POST["username"]) && isset($_POST["password"])) {
        // Open the DB
        $airplane = Airplane::get();

        // Create the new user
        $msg = $airplane->createUser($_POST["username"], $_POST["password"]);

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

    <link rel="stylesheet" type="text/css" href="css/style.css">
</head>
<body>
    <div class="container">
        <header>
            <h1><img class="icon-big" src="img/airplane.svg"><span>Airplane seats reservation system</span></h1>
        </header>
    </div>

    <div id="main" class="container hidden">
        <div class="row">
            <div class="menu">
                <h2>Menu</h2>
                <a class="menu-element" href="."><img class="icon" src="img/home.svg"> <span>Home</span></a>
<?php
    if ($session->getStatus() !== Session::STATUS_OK) {
?>
                <a class="menu-element" href="login.php"><img class="icon" src="img/login.svg"> <span>Login</span></a>
                <a class="menu-element" href="register.php"><img class="icon" src="img/register.svg"> <span>Register</span></a>
<?php
    } else {
?>
                <a class="menu-element" href="#" onclick="seats.requestUpdate();"><img class="icon" src="img/update.svg"> <span>Update</span></a>
                <a class="menu-element" href="#" onclick="seats.requestPurchase();"><img class="icon" src="img/purchase.svg"> <span>Purchase</span></a>
                <a class="menu-element" href="logout.php"><img class="icon" src="img/logout.svg"> <span>Logout</span></a>
<?php
    }
?>
            </div>

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

    <script type="application/javascript" src="js/jquery-3.4.1.min.js"></script>
    <script type="application/javascript" src="js/login.js"></script>
    <script type="application/javascript"><!--
        $(document).ready(function() {
            if (!navigator.cookieEnabled) {
                // Show the "Cookies are disabled" message
                $("body").append("<div class=\"container\">Please, enable cookies to visit this page.</div>");
            } else {
                // Show the page contents
                $("#main").removeClass("hidden");
            }

<?php
    if (isset($msg) && !$msg->getSuccess()) {
?>
            alert("<?= $msg->getMessage() ?>");
<?php
    } 
?>
        });
    //--></script>
    <noscript>
        <!-- Show the "Javascript is disabled" message -->
        <div class="container">Please, enable Javascript to visit this page.</div>
    </noscript>
</body>
</html>