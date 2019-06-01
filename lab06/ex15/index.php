<?php
    session_start();
    
    if (isset($_SESSION["jokenum"]))
        $jokenum = intval($_SESSION["jokenum"]);
    else
        $jokenum = 0;
    
    if ($jokenum > 0 && isset($_POST["jokeexit"]))
        $_SESSION["jokenum"] = $jokenum - 1;
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
    <h1>Want to hear a wonderful joke?</h1>
    <form action="joke.php" method="post">
        <input type="hidden" name="jokeenter">
        <input type="submit" value="Enter">
    </form>
</body>
</html>