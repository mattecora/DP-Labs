<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Lab 6.1.4</title>
</head>
<body>
    <h1>Lorem ipsum</h1>
    <?php
        if (isset($_COOKIE["name"]) && isset($_COOKIE["surname"]) && !isset($_POST["name"]) && !isset($_POST["surname"])) {
            echo "<p>Welcome back, $_COOKIE[name] $_COOKIE[surname]</p>";
        } else if (isset($_POST["name"]) && isset($_POST["surname"])) {
            setcookie("name", $_POST["name"]);
            setcookie("surname", $_POST["surname"]);
        }
    ?>
    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.</p>
    <p><a href="index.php">Go back</a></p>
    <p><a href="result.php">Refresh</a></p>
</body>
</html>