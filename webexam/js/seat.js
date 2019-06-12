/**
    seat.js
    Provides the classes used to represent the seats, organized according to the MVC pattern
    Matteo Corain - Distributed programming I - A.Y. 2018-19
*/

const STATUS_FREE = 0;
const STATUS_RESERVED = 1;
const STATUS_PURCHASED = 2;
const STATUS_SELECTED = 3;

function SeatModel(seatNum, status) {
    this.seatNum = seatNum;
    this.status = status;

    this.getSeatNum = function() {
        return this.seatNum;
    };

    this.getStatus = function() {
        return this.status;
    };

    this.setSeatNum = function(seatNum) {
        this.seatNum = seatNum;
    };

    this.setStatus = function(status) {
        this.status = status;
    };
}

function SeatView(seatModel, seatElement) {
    this.seatModel = seatModel;
    this.seatElement = seatElement;

    this.render = function() {
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

    this.render();
}

function SeatController(seatModel, seatView) {
    this.seatModel = seatModel;
    this.seatView = seatView;

    this.update = function(status) {
        // Update the model and render the view
        this.seatModel.setStatus(status);
        this.seatView.render(status);
    };
}