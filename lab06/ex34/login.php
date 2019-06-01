<?php
    $db = new mysqli("localhost", "root", "", "dp");
    if ($db->connect_errno) {
        die("Cannot connect to the database");
    }

    $query = $db->prepare("SELECT COUNT(*) FROM users WHERE username = ? AND PASSWORD = ?");
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
    <?php
        if (!isset($_POST["username"]) || !isset($_POST["password"])) {
    ?>
        <h1>Login</h1>
        <form action="login.php" method="post">
            <p>Username: <input type="text" name="username" id="username"></p>
            <p>Password: <input type="password" name="password" id="password"></p>
            <p><input type="submit" value="Submit"></p>
        </form>
    <?php
        } else {
            $md5 = md5($_POST["password"]);
            $query->bind_param("ss", $_POST["username"], $md5);
            $query->execute();

            if ($query->get_result()) {
                session_start();
                $_SESSION["username"] = $_POST["username"];
                $_SESSION["password"] = $md5;
    ?>
                <h1>You logged in successfully!</h1>
    <?php
            } else {
    ?>
                <h1>Invalid username or password!</h1>
    <?php
            }
        }
    ?>
</body>
</html>

<?php $db->close(); ?>