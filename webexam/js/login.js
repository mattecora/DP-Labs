/* Functions for the management of the login page */

function checkRegistration() {
    // Read username and password
    var username = $("#username").val();
    var password = $("#password").val();

    // Check username is a valid email
    if (!/^[a-zA-z0-9]+@[a-zA-z0-9]+\.[a-zA-z0-9]+$/.test(username)) {
        alert("Invalid username provided!");
        return false;
    }

    // Check password format
    if (!/[a-z]/.test(password) || !/[0-9A-Z]/.test(password)) {
        alert("Invalid password provided!");
        return false;
    }
    
    // Validation ok
    return true;
}