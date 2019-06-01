<?php
    $db = new mysqli("localhost", "root", "", "dp");
    if ($db->connect_errno) {
        die("Cannot connect to the database");
    }

    $ordered = array();
    $query = $db->prepare("SELECT * FROM books WHERE id = ?");
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Lab 6.3.4</title>

    <style>
        table, th, td {
            border: 1px solid black;
        }
    </style>
</head>
<body>
    <h1>Confirm order</h1>
    <form action="order.php" method="post">
        <table>
            <thead>
                <th>Product</th>
                <th>Quantity</th>
            </thead>
            <tbody>
                <?php
                    foreach ($_POST as $id => $qty) {
                        if ($qty > 0) {
                            $query->bind_param("i", $id);
                            $query->execute();

                            $res = $query->get_result();
                            if (!$res)
                                continue;
                            $res = $res->fetch_assoc();
                ?>
                <tr>
                    <td><?php echo $res["Title"]; ?></td>
                    <td><input type="number" name="<?php echo $id; ?>" id="<?php echo $id; ?>" value="<?php echo $qty; ?>" readonly></td>
                </tr>
                <?php
                        }
                    }
                ?>
            </tbody>
        </table>
        <input type="submit" value="Confirm">
    </form>
    <a href="index.php"><button>Cancel</button></a>
</body>
</html>

<?php $db->close(); ?>