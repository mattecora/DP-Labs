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

    <style>
        table, th, td {
            border: 1px solid black;
        }
    </style>
</head>
<body>
    <h1>Products list</h1>
    <form action="cart.php" method="post">
        <table>
            <thead>
                <th>Name</th>
                <th>Quantity</th>
            </thead>
            <tbody>
                <?php foreach ($products as $num => $p) { ?>
                <tr>
                    <td><a href="show.php?id=<?php echo $num; ?>"><?php echo $p->get_name(); ?></a></td>
                    <td><input type="number" name="<?php echo $num; ?>" id="<?php echo $num; ?>"></td>
                </tr>
                <?php } ?>
            </tbody>
        </table>
        <input type="hidden" name="fromindex">
        <input type="submit" value="Add to cart">
    </form>
    <a href="cart.php"><button>Shopping cart</button></a>
</body>
</html>