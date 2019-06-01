// Data starting point
let start = 0;

// DOM elements
let booksTable = document.getElementById("books");
let rangeP = document.getElementById("range");
let nextButton = document.getElementById("next");

function showNextData() {
    // Empty table and update range paragraph
    booksTable.innerHTML = "";
    rangeP.innerHTML = "Showing data from " + start + " to " + (start + 10);

    for (let i = start; i < start + 10; i++) {
        if (data[i] == undefined) {
            // Hide the show next button and return
            nextButton.style.display = "none";
            return;
        }
        
        // Create a row and insert it into the table
        let row = document.createElement("tr");
        
        let title = document.createElement("td");
        title.innerHTML = data[i].title;
        let author = document.createElement("td");
        author.innerHTML = data[i].author;
        let genre = document.createElement("td");
        genre.innerHTML = data[i].genre;
        let price = document.createElement("td");
        price.innerHTML = data[i].price;

        row.appendChild(title);
        row.appendChild(author);
        row.appendChild(genre);
        row.appendChild(price);

        booksTable.appendChild(row);
    }

    // Update starting point
    start = start + 10;
}

showNextData();