<?php
    $db = new mysqli("localhost", "root", "", "dp");
    if ($db->connect_errno) {
        die("Cannot connect to the database");
    }

    $ordered = array();
    $query = $db->prepare("SELECT * FROM books WHERE id = ?");
    $update = $db->prepare("UPDATE books SET Quantity = ? WHERE id = ?");

    foreach ($_POST as $id => $qty) {
        $query->bind_param("i", $id);
        $query->execute();

        $res = $query->get_result();
        if (!$res)
            continue;
        $res = $res->fetch_assoc();

        $newqty = intval($res["Quantity"]) - intval($qty);

        if ($qty <= 0 || $newqty < 0)
            continue;
        
        $update->bind_param("ii", $newqty, $id);
        $update->execute();

        $res["Quantity"] = $newqty;
        $ordered[] = $res;
    }

    $db->close();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Lab 6.3.4</title>
</head>
<body>
    <h1>Order successful</h1>
    <?php foreach ($ordered as $prod) { ?>
        <p>You ordered <?php echo $_POST[$prod["id"]] ?> copies of "<?php echo $prod["Title"]; ?>"</p>
        <p>Total price: <?php echo intval($_POST[$prod["id"]]) * $prod["Price"]; ?></p>
    <?php } ?>
    <a href="index.php">Go back</a>
</body>
</html>