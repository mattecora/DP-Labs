<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Lab 6.1.2</title>

    <style>
        table, th, td {
            border: 1px solid black;
        }
    </style>
</head>
<body>
    <h1>Cookies</h1>
    <table>
        <thead>
            <th>Cookie name</th>
            <th>Cookie value</th>
        </thead>
        <tbody>
            <?php foreach ($_COOKIE as $key => $value) { ?>
            <tr>
                <td>
                    <?php echo $key; ?>
                </td>
                <td>
                    <?php echo $value; ?>
                </td>
            </tr>
            <?php } ?>
        </tbody>
    </table>
</body>
</html>