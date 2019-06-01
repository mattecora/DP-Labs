<?php
    require("products.php");

    session_start();

    foreach ($products as $prod) {
        if (isset($_POST[$prod->get_name()])) {
            if (isset($_POST["fromindex"]) && isset($_SESSION[$prod->get_name()])) {
                $_SESSION[$prod->get_name()] = intval($_SESSION[$prod->get_name()]) + intval($_POST[$prod->get_name()]);
            } else {
                $_SESSION[$prod->get_name()] = $_POST[$prod->get_name()];
            }
        } else if (!isset($_SESSION[$prod->get_name()])) {
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

    <style>
        table, th, td {
            border: 1px solid black;
        }
    </style>
</head>
<body>
    <h1>Shopping cart</h1>
    <form action="cart.php" method="post">
        <table>
            <thead>
                <th>Name</th>
                <th>Quantity</th>
            </thead>
            <tbody>
                <?php
                    foreach ($products as $prod) {
                        $qty = intval($_SESSION[$prod->get_name()]);
                ?>
                <tr>
                    <td><?php echo $prod->get_name(); ?></td>
                    <td><input type="number" name="<?php echo $prod->get_name(); ?>" id="<?php echo $prod->get_name(); ?>" value="<?php echo $qty; ?>"></td>
                </tr>
                <?php } ?>
            </tbody>
        </table>
        <input type="submit" value="Update">
    </form>
    <a href="buy.php"><button>Buy</button></a>
    <a href="index.php"><button>Products</button></a>
</body>
</html>