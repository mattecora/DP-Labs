<div class="menu">
    <h2>Menu</h2>
    <a class="menu-element" href="index.php">Index</a>
    <?php if (!user_is_logged()) { ?>
        <a class="menu-element" href="login.php">Login</a>
        <a class="menu-element" href="register.php">Register</a>
    <?php } else { ?>
        <a class="menu-element" href="#" onclick="updateSeatMapHandler();">Update</a>
        <a class="menu-element" href="#" onclick="purchaseSeatsHandler();">Purchase</a>
        <a class="menu-element" href="logout.php">Logout</a>
    <?php } ?>
</div>