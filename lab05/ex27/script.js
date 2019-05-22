// DOM elements
let floppyNum = document.getElementById("floppyNum");
let cdNum = document.getElementById("cdNum");
let dvdNum = document.getElementById("dvdNum");
let totalText = document.getElementById("totalText");

function computeTotal() {
    // Compute the total
    let total = 0.5 * floppyNum.value + 1.0 * cdNum.value + 2.0 * dvdNum.value;

    // Set DOM element
    totalText.value = total;
}

function cleanUp() {
    // Reset value of fields
    floppyNum.selectedIndex = 0;
    cdNum.selectedIndex = 0;
    dvdNum.selectedIndex = 0;
    totalText.value = "";
}