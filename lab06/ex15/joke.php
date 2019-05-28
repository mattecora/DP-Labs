<?php
    if (isset($_COOKIE["jokenum"]))
        $jokenum = intval($_COOKIE["jokenum"]);
    else
        $jokenum = 0;
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Lab 6.1.5</title>
</head>
<body>
    <h1>A wonderful joke</h1>
    <?php
        if ($jokenum < 2 && isset($_POST["jokeenter"])) {
            setcookie("jokenum", $jokenum + 1);
    ?>
    <p style="font-style: italic;">Did you hear about the mathematician who’s afraid of negative numbers?</p>
    <p>He’ll stop at nothing to avoid them.</p>
    <?php
        } else {
    ?>
    <p style="font-weight: bold;">Sorry, you are already reading enough jokes!</p>
    <?php
        }
    ?>
    <form action="index.php" method="post">
        <?php if ($jokenum < 2) { ?>
        <input type="hidden" name="jokeexit">
        <?php } ?>
        <input type="submit" value="Exit">
    </form>
</body>
</html>