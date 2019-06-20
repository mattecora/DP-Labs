/**
    ajax.js
    Provides the functions for performing the needed AJAX interactions
    Matteo Corain - Distributed programming I - A.Y. 2018-19
*/

function ajaxError(xhr, status, error) {
    if (xhr.status === 400) {
        alert("Invalid request.");
    } else if (xhr.status === 401) {
        window.location = "login.php?expired";
    }
}

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
        error: ajaxError
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
        error: ajaxError
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
        error: ajaxError
    });
}

function ajaxFreeSeat(seat, callback) {
    $.ajax("api/free_seat.php", {
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
        error: ajaxError
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
        error: ajaxError
    });
}