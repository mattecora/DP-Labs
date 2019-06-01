<?php
    // Check name format
    function checkName($name) {
        $nameRegex = "/[A-Z][a-z]{1,9}/";
        return preg_match($nameRegex, $name);
    }

    // Check age value
    function checkAge($age) {
        return intval($age) >= 0 && intval($age) <= 199;
    }

    // Check phone format
    function checkPhone($phone) {
        $phoneRegex = "/0[0-9]{1,2}-[0-9]{3}-?[0-9]{3,4}/";
        return preg_match($phoneRegex, $phone);
    }
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Lab 6.2</title>
</head>
<body>
    <h1>Your data</h1>
    <?php
        if (!isset($_POST["name"]) || !isset($_POST["age"]) || !isset($_POST["phone"])) {
            echo "<p>Data not available.</p>";
        } else {
            if (checkName($_POST["name"]))
                echo "<p><strong>Name:</strong> $_POST[name]</p>";
            else
                echo "<p><strong>Name:</strong> invalid</p>";
            
            if (checkAge($_POST["age"]))
                echo "<p><strong>Age:</strong> $_POST[age]</p>";
            else
                echo "<p><strong>Age:</strong> invalid</p>";

            if (checkPhone($_POST["phone"]))
                echo "<p><strong>Phone:</strong> $_POST[phone]</p>";
            else
                echo "<p><strong>Phone:</strong> invalid</p>";
        }
    ?>
    <a href="index.php">Go back</a>
</body>
</html>