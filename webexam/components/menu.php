<div class="menu">
    <h2>Menu</h2>
    <a class="menu-element" href="."><img class="icon" src="img/home.svg"> <span>Home</span></a>
    <?php if (!user_is_logged()) { ?>
        <a class="menu-element" href="login.php"><img class="icon" src="img/login.svg"> <span>Login</span></a>
        <a class="menu-element" href="register.php"><img class="icon" src="img/register.svg"> <span>Register</span></a>
    <?php } else { ?>
        <a class="menu-element" href="#" onclick="seats.requestUpdate();"><img class="icon" src="img/update.svg"> <span>Update</span></a>
        <a class="menu-element" href="#" onclick="seats.requestPurchase();"><img class="icon" src="img/purchase.svg"> <span>Purchase</span></a>
        <a class="menu-element" href="logout.php"><img class="icon" src="img/logout.svg"> <span>Logout</span></a>
    <?php } ?>
</div>