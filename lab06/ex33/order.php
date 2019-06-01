<?php
    if (!isset($_POST["id"]) || !isset($_POST["qty"])) {
        die("Please select a product and a quantity");
    }

    $db = new mysqli("localhost", "root", "", "dp");
    if ($db->connect_errno) {
        die("Cannot connect to the database");
    }

    $query = $db->prepare("SELECT * FROM books WHERE id = ?");
    $query->bind_param("i", $_POST["id"]);
    $query->execute();
    $res = $query->get_result()->fetch_assoc();
    $db->close();

    if (!$res) {
        die("ID is not present in the database");
    }
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Lab 6.3.3</title>
</head>
<body>
    <h1>Order successful</h1>
    <p>You ordered <?php echo $_POST["qty"] ?> copies of "<?php echo $res["Title"]; ?>"</p>
    <p>Total price: <?php echo intval($_POST["qty"]) * $res["Price"]; ?></p>
</body>
</html>