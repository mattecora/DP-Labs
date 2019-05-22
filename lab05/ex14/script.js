// Retrieve the ps in the DOM
let numberText = document.getElementById("numberText");
let sumText = document.getElementById("sumText");
let diffText = document.getElementById("diffText");

// Ask for two numbers
let number1 = Number.parseInt(prompt("Insert first number", 0));
let number2 = Number.parseInt(prompt("Insert second number", 0));

// Compute sum and diff
let sum = number1 + number2;
let diff = number1 - number2;

// Set the DOM elements
numberText.innerHTML = "You inserted " + number1 + " and " + number2;
sumText.innerHTML = "The sum is " + sum;
diffText.innerHTML = "The difference is " + diff;