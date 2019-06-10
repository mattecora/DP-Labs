function ajaxGetSeatStatus(seat, callback) {
    $.ajax("api/get_seat_status.php", {
        method: "POST",
        data: {
            seat: seat
        },
        dataType: "json",
        success: function(response, status, xhr) {
            if (response["data"]) {
                callback(response["data"]);
            }
            if (response["message"]) {
                alert(response["message"]);
            }
        },
        error: function(xhr, status, error) {
            if (xhr.status === 400) {
                alert("Your request was invalid!");
            } else if (xhr.status === 401) {
                alert("Your session has expired!");
                window.location = "index.php";
            }
        }
    });
}

function ajaxGetSeatStatusAll(callback) {
    $.ajax("api/get_seat_status.php", {
        method: "GET",
        dataType: "json",
        success: function(response, status, xhr) {
            if (response["data"]) {
                callback(response["data"]);
            }
            if (response["message"]) {
                alert(response["message"]);
            }
        },
        error: function(xhr, status, error) {
            if (xhr.status === 400) {
                alert("Your request was invalid!");
            } else if (xhr.status === 401) {
                alert("Your session has expired!");
                window.location = "index.php";
            }
        }
    });
}

function ajaxReserveSeat(seat, callback) {
    $.ajax("api/request_seat.php", {
        method: "POST",
        data: {
            seat: seat
        },
        dataType: "json",
        success: function(response, status, xhr) {
            if (response["data"]) {
                callback(response["data"]);
            }
            if (response["message"]) {
                alert(response["message"]);
            }
        },
        error: function(xhr, status, error) {
            if (xhr.status === 400) {
                alert("Your request was invalid!");
            } else if (xhr.status === 401) {
                alert("Your session has expired!");
                window.location = "index.php";
            }
        }
    });
}

function ajaxPurchaseSeats(seats, callback) {
    $.ajax("api/purchase_seats.php", {
        method: "POST",
        data: {
            seats: seats
        },
        dataType: "json",
        success: function(response, status, xhr) {
            if (response["data"]) {
                callback(response["data"]);
            }
            if (response["message"]) {
                alert(response["message"]);
            }
        },
        error: function(xhr, status, error) {
            if (xhr.status === 400) {
                alert("Your request was invalid!");
            } else if (xhr.status === 401) {
                alert("Your session has expired!");
                window.location = "index.php";
            }
        }
    });
}