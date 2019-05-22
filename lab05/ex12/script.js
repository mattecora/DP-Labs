// Retrieve the p in the DOM
let numberText = document.getElementById("numberText");

// Ask for a number
let number = prompt("Insert a number", 0);

// Check parity
if (number % 2 == 0) {
    numberText.innerHTML = "You inserted: " + number + " (even)";
} else {
    numberText.innerHTML = "You inserted: " + number + " (odd)";
}