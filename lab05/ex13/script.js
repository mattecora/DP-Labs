// Ask for two numbers
let number1 = Number.parseInt(prompt("Insert first number", 0));
let number2 = Number.parseInt(prompt("Insert second number", 0));

// Compute sum and diff
let sum = number1 + number2;
let diff = number1 - number2;

// Show results
alert("You inserted " + number1 + " and " + number2);
alert("The sum is " + sum);
alert("The difference is " + diff);