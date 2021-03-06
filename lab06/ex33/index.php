<?php
    $db = new mysqli("localhost", "root", "", "dp");
    if ($db->connect_errno) {
        die("Cannot connect to the database");
    }

    $query = $db->query("SELECT * FROM books");
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Lab 6.3.3</title>

    <style>
        table, th, td {
            border: 1px solid black;
        }
    </style>
</head>
<body>
    <h1>Place a order</h1>
    <form action="order.php" method="post">
        <input type="number" name="id" id="id" placeholder="ID">
        <input type="number" name="qty" id="qty" placeholder="Quantity">
        <input type="submit" value="Place">
    </form>

    <h1>Books library</h1>
    <table>
        <thead>
            <th>ID</th>
            <th>Title</th>
            <th>Author</th>
            <th>Genre</th>
            <th>Price</th>
        </thead>
        <tbody>
            <?php while ($row = $query->fetch_assoc()) { ?>
                <tr>
                    <td><?php echo $row["id"]; ?></td>
                    <td><?php echo $row["Title"]; ?></td>
                    <td><?php echo $row["Author"]; ?></td>
                    <td><?php echo $row["Genre"]; ?></td>
                    <td><?php echo $row["Price"]; ?></td>
                </tr>
            <?php } ?>
        </tbody>
    </table>
</body>
</html>

<?php $db->close(); ?>