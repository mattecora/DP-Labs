<?php
    require("products.php");

    session_start();

    foreach ($products as $prod) {
        if (!isset($_SESSION[$prod->get_name()])) {
            $_SESSION[$prod->get_name()] = 0;
        }
    }
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Lab 6.1.6</title>
</head>
<body>
    <h1>Transaction confirmed!</h1>
    <?php
        if (isset($_POST["total"])) {
    ?>
    <p>Total price: <?php echo $_POST["total"]; ?></p>
    <?php } ?>
    <a href="index.php"><button>Go to index</button></a>
</body>
</html>