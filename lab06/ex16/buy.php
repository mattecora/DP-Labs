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

    <style>
        table, th, td {
            border: 1px solid black;
        }
    </style>
</head>
<body>
    <h1>Selected products</h1>
    <form action="confirm.php" method="post">
        <table>
            <thead>
                <th>Name</th>
                <th>Quantity</th>
                <th>Total price</th>
            </thead>
            <tbody>
                <?php
                    $sum = 0;
                    foreach ($products as $prod) {
                        $qty = $_SESSION[$prod->get_name()];
                        $sum = $sum + $qty * $prod->get_price();
                ?>
                <tr>
                    <td><?php echo $prod->get_name(); ?></td>
                    <td><?php echo $qty; ?></td>
                    <td><?php echo $qty * $prod->get_price(); ?> &euro;</td>
                </tr>
                <?php } ?>
            </tbody>
        </table>
        Total price: <input type="text" name="total" id="total" value="<?php echo $sum; ?> &euro;" readonly>
        <input type="submit" value="Confirm">
    </form>
</body>
</html>