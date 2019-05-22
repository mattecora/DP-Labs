function computeSum() {
    // Get the DOM elements
    let firstNum = Number.parseInt(document.getElementById("firstNum").value);
    let secondNum = Number.parseInt(document.getElementById("secondNum").value);

    // Compute sum and set DOM element
    document.getElementById("resultNum").value = firstNum + secondNum;
}