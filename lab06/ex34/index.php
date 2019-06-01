<?php
    session_start();

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
    <title>Lab 6.3.4</title>

    <style>
        table, th, td {
            border: 1px solid black;
        }
    </style>
</head>
<body>
    <h1>Books library</h1>
    <p>
        <?php
            if (isset($_SESSION["username"]) && isset($_SESSION["password"])) {
        ?>
            You are logged in as: <?php echo $_SESSION["username"]; ?>
        <?php
            } else {
        ?>
            Not logged in! <a href="login.php">Login</a>
        <?php
            }
        ?>
    </p>
    <form action="confirm.php" method="post">
        <input type="submit" value="Place order">
        <table>
            <thead>
                <th>ID</th>
                <th>Title</th>
                <th>Author</th>
                <th>Genre</th>
                <th>Price</th>
                <th>Quantity</th>
                <th>Ordered</th>
            </thead>
            <tbody>
                <?php while ($row = $query->fetch_assoc()) { ?>
                    <tr>
                        <td><?php echo $row["id"]; ?></td>
                        <td><?php echo $row["Title"]; ?></td>
                        <td><?php echo $row["Author"]; ?></td>
                        <td><?php echo $row["Genre"]; ?></td>
                        <td><?php echo $row["Price"]; ?></td>
                        <td><?php echo $row["Quantity"]; ?></td>
                        <?php
                            if (isset($_SESSION["username"]) && isset($_SESSION["password"])) {
                        ?>
                            <td><input type="number" name="<?php echo $row["id"]; ?>" id="<?php echo $row["id"]; ?>" max="<?php echo $row["Quantity"]; ?>" min="0"></td>
                        <?php
                            }
                        ?>
                    </tr>
                <?php } ?>
            </tbody>
        </table>
    </form>
</body>
</html>

<?php $db->close(); ?>