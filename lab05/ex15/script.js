// Get DOM elements
let titleH = document.getElementById("title");
let squaresTable = document.getElementById("squares");

// Ask for a number
let number = Number.parseInt(prompt("Enter a number", 5));

// Write title
titleH.innerHTML = "Squares of numbers up to " + number;

// Compute squares
for (let i = 1; i <= number; i++) {
    let tr = document.createElement("tr");
    let td1 = document.createElement("td");
    let td2 = document.createElement("td");
    let td3 = document.createElement("td");

    td1.innerHTML = i;
    td2.innerHTML = i * i;
    td3.innerHTML = i * i * i;

    tr.appendChild(td1);
    tr.appendChild(td2);
    tr.appendChild(td3);
    squaresTable.appendChild(tr);
}