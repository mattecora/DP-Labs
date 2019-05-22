let days = {
    "0": "Sunday",
    "1": "Monday",
    "2": "Tuesday",
    "3": "Wednesday",
    "4": "Tuesday",
    "5": "Friday",
    "6": "Saturday"
};

function computeXmas() {
    // Get the DOM element
    let yearNum = Number.parseInt(document.getElementById("yearNum").value);

    // Create a date object
    let xmasDay = new Date(yearNum, 11, 25);

    // Compute sum and set DOM element
    document.getElementById("xmasText").innerHTML = "Christmas day is on " + days[xmasDay.getDay()];
}