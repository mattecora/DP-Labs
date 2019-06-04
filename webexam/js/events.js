/* Functions used as event handlers */

function selectSeatHandler(seat) {
    let seatTd = $("#" + seat);

    // Check if the user is logged in
    if (username === undefined) {
        alert("Seat reservation is for registered users only!");
        return;
    }

    // Check if the seat is already purchased
    if (seatTd.hasClass("purchased-seat")) {
        alert("Seat " + seat + " has already been purchased!");
        return;
    }
    
    // Check if the seat has been purchased in the meantime
    getSeatStatus(seat, (data, status, xhr) => {
        seats[seat] = data[seat];

        if (data[seat]["status"] === SEAT_PURCHASED) {
            // Seat has been purchased in the meantime
            alert("Seat " + seat + " has already been purchased!");
            updateSeat(seat, data[seat]["status"], data[seat]["reserver"]);
            return;
        }

        // Check if the seat is selected or not
        if (data[seat]["status"] === SEAT_RESERVED && data[seat]["reserver"] === username) {
            // Seat is selected and must be freed
            freeSeat(seat, (data2, status2, xhr2) => {
                // Seat freed
                seats[seat] = data2[seat];
                updateSeat(seat, SEAT_FREE, null);
            });
        } else {
            // Seat is free and must be selected
            reserveSeat(seat, (data2, status2, xhr2) => {
                // Seat reserved
                seats[seat] = data2[seat];
                updateSeat(seat, SEAT_RESERVED, username);
            });
        }
    });
}

function updateSeatMapHandler() {
    // Retrieve the seat map
    getSeatStatusAll((data, status, xhr) => {
        // Update the seat array and the view
        seats = data;
        updateSeatMap();
    });
}

function purchaseSeatsHandler() {
    let toPurchase = [];

    // Check all seats to purchase
    for (let seat in seats) {
        if (seats[seat]["status"] === SEAT_RESERVED && seats[seat]["reserver"] === username)
            toPurchase.push(seat);
    }

    // Request the purchase
    purchaseSeats(toPurchase, (data, status, xhr) => {
        // Update the seat array and the view
        seats = data;
        updateSeatMap();
    });
}