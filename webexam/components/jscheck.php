<noscript>
    <!-- Show the "Javascript is disabled" message -->
    <div class="container">Please, enable Javascript to visit this page.</div>
</noscript>

<script>
    if (!navigator.cookieEnabled) {
        // Show the "Cookies are disabled" message
        document.write("<div class=\"container\">Please, enable cookies to visit this page.</div>");
    } else {
        // Show the page contents
        document.getElementById("main").classList.remove("hidden");
    }
</script>