<?php
    require("products.php");

    foreach ($products as $num => $prod) {
        if (!isset($_COOKIE[$num])) {
            setcookie($num, 0);
            $_COOKIE[$num] = 0;
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
    <?php
        if (isset($_GET["id"]) && ($id = intval($_GET["id"])) < sizeof($products)) {
            $prod = $products[intval($_GET["id"])];
    ?>
    <h1><?php echo $prod->get_name(); ?></h1>
    <p><strong>Description:</strong> <?php echo $prod->get_desc(); ?></p>
    <p><strong>Price:</strong> <?php echo $prod->get_price(); ?> &euro;</p>
    <?php } else { ?>
    <h1>Invalid product</h1>
    <?php } ?>
    <a href="index.php">Go back</a>
</body>
</html>