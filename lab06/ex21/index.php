<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Lab 6.2</title>
</head>
<body>
    <h1>Input your data</h1>
    <form action="send.php" method="post" onsubmit="checkFields()">
        <table>
            <tr>
                <td>Name:</td>
                <td><input type="text" name="name" id="name" maxlength="10"></td>
            </tr>
            <tr>
                <td>Age:</td>
                <td><input type="number" name="age" id="age" min="0" max="199"></td>
            </tr>
            <tr>
                <td>Phone:</td>
                <td><input type="text" name="phone" id="phone" maxlength="12"></td>
            </tr>
        </table>
        <input type="submit" value="Send">
    </form>

    <script src="script.js"></script>
</body>
</html>