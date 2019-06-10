<?php
    require_once "app/enforce_https.php";

    require_once "app/db.php";
    require_once "app/seatmap.php";
    require_once "app/session.php";

    $db = new AirplaneDatabase();
    $seatmap = $db->getSeatStatusAll();
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
    <?php
        if (session_start_timeout() == SESSION_EXPIRED)
            echo "<script>alert(\"Your session has expired!\");</script>";
    ?>

    <div class="container">
        <?php require_once "components/header.php"; ?>
    </div>

    <div id="main" class="container hidden">
        <div class="row">
            <?php require_once "components/menu.php"; ?>

            <div class="contents">
                <div class="row">
                    <div class="seatmap">
                        <h2>Seat map</h2>
                        <table>
                            <?php for ($row = 0; $row < SeatMap::ROWS; $row++) { ?>
                                <tr>
                                    <?php
                                        for ($col = 1; $col <= SeatMap::PLACES; $col++) {
                                            $seatid = chr($row + ord('A')) . $col;
                                            if (user_is_logged()) {
                                    ?>
                                        <td id="<?= $seatid ?>" class="seat seat-clickable" onclick="seats.requestSelect('<?= $seatid ?>')">
                                            <img class="icon" src="img/seat.svg"><span><?= $seatid ?></span>
                                        </td>
                                    <?php } else { ?>
                                        <td id="<?= $seatid ?>" class="seat">
                                            <img class="icon" src="img/seat.svg"><span><?= $seatid ?></span>
                                        </td>
                                    <?php
                                        }
                                    } 
                                    ?>
                                </tr>
                            <?php } ?>
                        </table>

                        <?php if (user_is_logged()) { ?>
                            <p>You are logged in as <?= $_SESSION["username"] ?></p>
                        <?php } ?>
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

    <?php require_once "components/jscheck.php"; ?>
    
    <script src="js/jquery-3.4.1.min.js"></script>
    <script src="js/ajax.js"></script>
    <script src="js/counter.js"></script>
    <script src="js/seat.js"></script>
    <script src="js/seatmap.js"></script>

    <script>
        var seats = new SeatMap(<?= json_encode($seatmap->getData()) ?>);

        <?php if (user_is_logged()) { ?>
            var username = "<?= $_SESSION["username"] ?>";
        <?php } else { ?>
            var username = undefined;
        <?php } ?>
    </script>
</body>
</html>