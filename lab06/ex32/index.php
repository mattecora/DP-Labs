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
    <title>Lab 6.3.2</title>

    <style>
        table, th, td {
            border: 1px solid black;
        }
    </style>
</head>
<body>
    <h1>Books library</h1>
    <p id="range"></p>
    <table>
        <thead>
            <th>Title</th>
            <th>Author</th>
            <th>Genre</th>
            <th>Price</th>
        </thead>
        <tbody id="books">
            
        </tbody>
    </table>

    <button id="next" onclick="showNextData()">Show next</button>

    <script>
        let data = [
            <?php
                while ($row = $query->fetch_assoc())
                    echo "{title: \"$row[Title]\", author: \"$row[Author]\", genre: \"$row[Genre]\", price: \"$row[Price]\"},";
            ?>
        ];
    </script>
    <script src="script.js"></script>
</body>
</html>

<?php $db->close(); ?>