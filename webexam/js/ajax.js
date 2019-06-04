/* Functons for performing AJAX requests */

function getSeatStatus(seat, callback) {
    $.ajax("api/get_seat_status.php", {
        method: "POST",
        data: {
            seat: seat
        },
        dataType: "json",
        success: callback,
        error: (xhr, status, error) => {
            if (error === "Unauthorized") {
                alert("Your session has expired!");
                window.location = "index.php";
            } else {
                alert("Your request was invalid!");
            }
        }
    });
}

function getSeatStatusAll(callback) {
    $.ajax("api/get_seat_status.php", {
        method: "GET",
        dataType: "json",
        success: callback,
        error: (xhr, status, error) => {
            if (error === "Unauthorized") {
                alert("Your session has expired!");
                window.location = "index.php";
            } else {
                alert("Your request was invalid!");
            }
        }
    });
}

function reserveSeat(seat, callback) {
    $.ajax("api/request_seat.php", {
        method: "POST",
        data: {
            seat: seat
        },
        dataType: "json",
        success: callback,
        error: (xhr, status, error) => {
            if (error === "Unauthorized") {
                alert("Your session has expired!");
                window.location = "index.php";
            } else {
                alert("Your request was invalid!");
            }
        }
    });
}

function freeSeat(seat, callback) {
    $.ajax("api/free_seat.php", {
        method: "POST",
        data: {
            seat: seat
        },
        dataType: "json",
        success: callback,
        error: (xhr, status, error) => {
            if (error === "Unauthorized") {
                alert("Your session has expired!");
                window.location = "index.php";
            } else {
                alert("Your request was invalid!");
            }
        }
    });
}

function purchaseSeats(seats, callback) {
    $.ajax("api/purchase_seats.php", {
        method: "POST",
        data: {
            seats: seats
        },
        dataType: "json",
        success: callback,
        error: (xhr, status, error) => {
            if (error === "Unauthorized") {
                alert("Your session has expired!");
                window.location = "index.php";
            } else {
                alert("Your request was invalid!");
            }
        }
    });
}