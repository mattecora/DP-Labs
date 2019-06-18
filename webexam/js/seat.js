/**
    seat.js
    Provides the classes used to represent the seats, organized according to the MVC pattern
    Matteo Corain - Distributed programming I - A.Y. 2018-19
*/

const STATUS_FREE = 0;
const STATUS_RESERVED = 1;
const STATUS_PURCHASED = 2;
const STATUS_SELECTED = 3;

// Seat model

function SeatModel(seatNum, status) {
    this.seatNum = seatNum;
    this.status = status;
}

SeatModel.prototype.getSeatNum = function() {
    return this.seatNum;
};

SeatModel.prototype.getStatus = function() {
    return this.status;
};

SeatModel.prototype.setSeatNum = function(seatNum) {
    this.seatNum = seatNum;
};

SeatModel.prototype.setStatus = function(status) {
    this.status = status;
};

// Seat view

function SeatView(seatModel, seatElement) {
    this.seatModel = seatModel;
    this.seatElement = seatElement;

    this.render();
}

SeatView.prototype.render = function() {
    // Remove all classes
    this.seatElement.removeClass("free-seat reserved-seat purchased-seat selected-seat");
    
    // Show new seat status
    if (this.seatModel.getStatus() === STATUS_FREE)
        this.seatElement.addClass("free-seat");
    else if (this.seatModel.getStatus() === STATUS_RESERVED)
        this.seatElement.addClass("reserved-seat");
    else if (this.seatModel.getStatus() === STATUS_PURCHASED)
        this.seatElement.addClass("purchased-seat");
    else if (this.seatModel.getStatus() === STATUS_SELECTED)
        this.seatElement.addClass("selected-seat");
};

// Seat controller

function SeatController(seatModel, seatView) {
    this.seatModel = seatModel;
    this.seatView = seatView;
}

SeatController.prototype.update = function(status) {
    // Update the model and render the view
    this.seatModel.setStatus(status);
    this.seatView.render(status);
};