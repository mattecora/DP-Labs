// DOM elements
let nameText = document.getElementById("name");
let ageText = document.getElementById("age");
let phoneText = document.getElementById("phone");

// Check name format
function checkName() {
    let nameRegex = /[A-Z][a-z]{1,9}/;
    return nameRegex.test(nameText.value);
}

// Check age value
function checkAge() {
    return Number.parseInt(ageText.value) >= 0 && Number.parseInt(ageText.value) <= 199;
}

// Check phone format
function checkPhone() {
    let phoneRegex = /0[0-9]{1,2}-[0-9]{3}-?[0-9]{3,4}/;
    return phoneRegex.test(phoneText.value);
}

// Check all fields and alert if wrong
function checkFields() {
    if (!checkName()) {
        alert("Invalid name!");
        return false;
    }
    if (!checkAge()) {
        alert("Invalid age!");
        return false;
    }
    if (!checkPhone()) {
        alert("Invalid phone!");
        return false;
    }
    return true;
}