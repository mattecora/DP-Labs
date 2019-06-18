<!--
    index.php
    Entry point of the website
    Matteo Corain - Distributed programming I - A.Y. 2018-19
-->

<?php
    require_once "app/airplane.php";
    require_once "app/seatmap.php";
    require_once "app/session.php";

    $session = new Session();

    $airplane = new Airplane();
    $seatmap = $airplane->getSeatStatusAll();
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
                <a class="menu-element" href="#" onclick="airplane.requestUpdate();"><img class="icon" src="img/update.svg"> <span>Update</span></a>
                <a class="menu-element" href="#" onclick="airplane.requestPurchase();"><img class="icon" src="img/purchase.svg"> <span>Purchase</span></a>
                <a class="menu-element" href="logout.php"><img class="icon" src="img/logout.svg"> <span>Logout</span></a>
<?php
    }
?>
            </div>

            <div class="contents">
                <div class="row">
                    <div class="seatmap">
                        <h2>Seat map</h2>
                        <table>
<?php
    for ($row = 0; $row < SeatMap::ROWS; $row++) {
?>
                            <tr>
<?php
        for ($place = 0; $place < SeatMap::PLACES; $place++) {
            $seatid = SeatMap::generateSeatNum($row, $place);
            if ($session->getStatus() === Session::STATUS_OK) {
?>
                                <td id="<?= $seatid ?>" class="seat seat-clickable" onclick="airplane.requestSelect('<?= $seatid ?>')">
                                    <img class="icon" src="img/seat.svg"><span><?= $seatid ?></span>
                                </td>
<?php
            } else {
?>
                                <td id="<?= $seatid ?>" class="seat">
                                    <img class="icon" src="img/seat.svg"><span><?= $seatid ?></span>
                                </td>
<?php
            }
        }
?>
                            </tr>
<?php
    }
?>
                        </table>
<?php
    if ($session->getStatus() === Session::STATUS_OK) { 
?>
                        <p>You are logged in as <?= $session->getUsername() ?></p>
<?php
    }
?>
                    </div>
                    
                    <div class="statistics">
                        <h2>Statistics</h2>
                        <p class="menu-element"><span>Total seats: </span><span id="totalSeats"><?= SeatMap::ROWS * SeatMap::PLACES ?></span></p>
                        <p class="menu-element"><span>Free seats: </span><span id="freeSeats">0</span></p>
                        <p class="menu-element"><span>Reserved seats: </span><span id="reservedSeats">0</span></p>
                        <p class="menu-element"><span>Purchased seats: </span><span id="purchasedSeats">0</span></p>
                    </div>
                </div>
            </div>
        </div>
    </div>
    
    <script type="application/javascript" src="js/jquery-3.4.1.min.js"></script>
    <script type="application/javascript" src="js/ajax.js"></script>
    <script type="application/javascript" src="js/counter.js"></script>
    <script type="application/javascript" src="js/seat.js"></script>
    
    <script type="application/javascript" src="js/airplane.js"></script>
    <script type="application/javascript"><!--
        var airplane = undefined;

        $(document).ready(function() {
            if (!navigator.cookieEnabled) {
                // Show the "Cookies are disabled" message
                $("body").append("<div class=\"container\">Please, enable cookies to visit this page.</div>");
            } else {
                // Show the page contents
                $("#main").removeClass("hidden");
            }
<?php
    if ($session->getStatus() === Session::STATUS_EXPIRED) {
?>
            alert("Expired session.");
<?php
    }
?>
            airplane = new Airplane(<?= json_encode($seatmap->getData()) ?>);
        });
    //--></script>
    <noscript>
        <!-- Show the "Javascript is disabled" message -->
        <div class="container">Please, enable Javascript to visit this page.</div>
    </noscript>
</body>
</html>