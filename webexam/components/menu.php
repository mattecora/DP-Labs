<div class="menu">
    <h2>Menu</h2>
    <a class="menu-element" href="."><img class="icon" src="img/home.svg"> <span>Home</span></a>
    <?php if (!user_is_logged()) { ?>
        <a class="menu-element" href="login.php"><img class="icon" src="img/login.svg"> <span>Login</span></a>
        <a class="menu-element" href="register.php">Register</a>
    <?php } else { ?>
        <a class="menu-element" href="#" onclick="seats.requestUpdate();">Update</a>
        <a class="menu-element" href="#" onclick="seats.requestPurchase();">Purchase</a>
        <a class="menu-element" href="logout.php">Logout</a>
    <?php } ?>
</div>