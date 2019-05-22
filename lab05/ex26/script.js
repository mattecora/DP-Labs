// DOM elements
let number1 = document.getElementById("number1");
let number2 = document.getElementById("number2")
let number3 = document.getElementById("number3")
let number4 = document.getElementById("number4")
let number5 = document.getElementById("number5")
let maxText = document.getElementById("maxText")

function greatest() {
    let max = Number.MIN_SAFE_INTEGER;

    // Loop through arguments and set max when necessary
    for (let i = 0; i < arguments.length; i++) {
        if (arguments[i] > max) {
            max = arguments[i];
        }
    }

    return max;
}

function computeMax() {
    // Get the maximum of the DOM elements values
    let maxNum = greatest(
        Number.parseInt(number1.value),
        Number.parseInt(number2.value),
        Number.parseInt(number3.value),
        Number.parseInt(number4.value),
        Number.parseInt(number5.value)
    );

    // Set DOM element
    maxText.innerHTML = "Maximum value is " + maxNum;
}

function cleanUp() {
    // Reset value of fields
    number1.value = "";
    number2.value = "";
    number3.value = "";
    number4.value = "";
    number5.value = "";
    maxText.innerHTML = "";
}