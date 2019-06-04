/* Functions for the management of the view */

function updateSeat(seat, status, reserver) {
    // Remove all classes and add the appropriate one
    $("#" + seat).removeClass("free-seat reserved-seat purchased-seat selected-seat");
    
    // Check new seat status
    if (status === SEAT_FREE)
        $("#" + seat).addClass("free-seat");
    else if (status === SEAT_RESERVED && username === reserver)
        $("#" + seat).addClass("selected-seat");
    else if (status === SEAT_RESERVED)
        $("#" + seat).addClass("reserved-seat");
    else
        $("#" + seat).addClass("purchased-seat");
    
    // TODO : Update counters
}

function updateSeatMap() {
    let counters = [0, 0, 0];

    // Update seats
    for (let seat in seats) {
        updateSeat(seat, seats[seat]["status"], seats[seat]["reserver"]);
        counters[seats[seat]["status"]]++;
    }

    // Update counters
    $("#totalSeats").html(counters[SEAT_FREE] + counters[SEAT_RESERVED] + counters[SEAT_PURCHASED]);
    $("#freeSeats").html(counters[SEAT_FREE]);
    $("#reservedSeats").html(counters[SEAT_RESERVED]);
    $("#purchasedSeats").html(counters[SEAT_PURCHASED]);
}