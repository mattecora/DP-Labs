// Get DOM elements
let numbersP = document.getElementById("numbers");
let meanP = document.getElementById("mean")

// Initialize empty numbers array
let numbers = [];
let sum = 0;
let count = 0;

// Ask for numbers
while (true) {
    let input = prompt("Enter a number", 5);
    if (input === "end") break;
    
    numbers.push(Number.parseInt(input));
    sum += Number.parseInt(input);
    count++;
}

// Output results
for (let n in numbers)
    numbersP.innerHTML += numbers[n] + " ";
meanP.innerHTML += sum / count;