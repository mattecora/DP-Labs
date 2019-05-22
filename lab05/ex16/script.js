// Get DOM elements
let titleH = document.getElementById("title");
let pythagTable = document.getElementById("pythag");

// Ask for a number
let number = Number.parseInt(prompt("Enter a number", 5));

// Write title
titleH.innerHTML = "Pythagorean table of order " + number;

// Output first line
let upperTr = document.createElement("tr");
for (let i = 0; i <= number; i++) {
    let th = document.createElement("th");
    th.innerHTML = i;
    upperTr.appendChild(th);
}
pythagTable.appendChild(upperTr);

// Compute table
for (let i = 1; i <= number; i++) {
    let tr = document.createElement("tr");
    
    let th = document.createElement("th");
    th.innerHTML = i;
    tr.appendChild(th);

    for (let j = 1; j <= number; j++) {
        let td = document.createElement("td");
        td.innerHTML = i * j;
        tr.appendChild(td);
    }

    pythagTable.appendChild(tr);
}