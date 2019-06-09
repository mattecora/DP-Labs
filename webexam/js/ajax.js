/* Functons for performing AJAX requests */

function getSeatStatus(seat, callback) {
    $.ajax("api/get_seat_status.php", {
        method: "POST",
        data: {
            seat: seat
        },
        dataType: "json",
        success: (response, status, xhr) => {
            if (response["data"]) {
                callback(response["data"]);
            }
            if (response["message"]) {
                alert(response["message"]);
            }
        },
        error: (xhr, status, error) => {
            if (xhr.status === 400) {
                alert("Your request was invalid!");
            } else if (xhr.status === 401) {
                alert("Your session has expired!");
                window.location = "index.php";
            }
        }
    });
}

function getSeatStatusAll(callback) {
    $.ajax("api/get_seat_status.php", {
        method: "GET",
        dataType: "json",
        success: (response, status, xhr) => {
            if (response["data"]) {
                callback(response["data"]);
            }
            if (response["message"]) {
                alert(response["message"]);
            }
        },
        error: (xhr, status, error) => {
            if (xhr.status === 400) {
                alert("Your request was invalid!");
            } else if (xhr.status === 401) {
                alert("Your session has expired!");
                window.location = "index.php";
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
        success: (response, status, xhr) => {
            if (response["data"]) {
                callback(response["data"]);
            }
            if (response["message"]) {
                alert(response["message"]);
            }
        },
        error: (xhr, status, error) => {
            if (xhr.status === 400) {
                alert("Your request was invalid!");
            } else if (xhr.status === 401) {
                alert("Your session has expired!");
                window.location = "index.php";
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
        success: (response, status, xhr) => {
            if (response["data"]) {
                callback(response["data"]);
            }
            if (response["message"]) {
                alert(response["message"]);
            }
        },
        error: (xhr, status, error) => {
            if (xhr.status === 400) {
                alert("Your request was invalid!");
            } else if (xhr.status === 401) {
                alert("Your session has expired!");
                window.location = "index.php";
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
        success: (response, status, xhr) => {
            if (response["data"]) {
                callback(response["data"]);
            }
            if (response["message"]) {
                alert(response["message"]);
            }
        },
        error: (xhr, status, error) => {
            if (xhr.status === 400) {
                alert("Your request was invalid!");
            } else if (xhr.status === 401) {
                alert("Your session has expired!");
                window.location = "index.php";
            }
        }
    });
}