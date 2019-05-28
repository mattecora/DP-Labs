<?php
    require("products.php");

    foreach ($products as $num => $prod) {
        if (isset($_POST[$num])) {
            if (isset($_POST["fromindex"]) && isset($_COOKIE[$num])) {
                setcookie($num, intval($_COOKIE[$num]) + intval($_POST[$num]));
                $_COOKIE[$num] = intval($_COOKIE[$num]) + intval($_POST[$num]);
            } else {
                setcookie($num, $_POST[$num]);
                $_COOKIE[$num] = $_POST[$num];
            }
        } else if (!isset($_COOKIE[$num])) {
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
                    foreach ($products as $num => $prod) {
                        $qty = intval($_COOKIE[$num]);
                ?>
                <tr>
                    <td><?php echo $prod->get_name(); ?></td>
                    <td><input type="number" name="<?php echo $num; ?>" id="<?php echo $num; ?>" value="<?php echo $qty; ?>"></td>
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