/**
    airplane.js
    Provides the class for representing the airplane, which handles user interactions
    Matteo Corain - Distributed programming I - A.Y. 2018-19
*/

function Airplane(data) {
    this.seats = {};

    var counterModel = new CounterModel();
    var counterView = new CounterView(counterModel, $("#freeSeats"), $("#reservedSeats"), $("#purchasedSeats"));
    this.counter = new CounterController(counterModel, counterView);

    for (var seat in data) {
        var seatModel = new SeatModel(seat, data[seat]);
        var seatView = new SeatView(seatModel, $("#" + seat));
        this.seats[seat] = new SeatController(seatModel, seatView);
    }

    this.counter.update(this.seats);
}

Airplane.prototype.update = function(seats) {
    // Update seats
    for (var seat in this.seats)
        this.seats[seat].update(seats[seat]);
    
    // Update counters
    this.counter.update(this.seats);
};

Airplane.prototype.requestSelect = function(seat) {
    var self = this;

    // Check seat status
    if (this.seats[seat].seatModel.getStatus() === STATUS_PURCHASED) {
        // Seat is purchased, fail
        alert("This seat is already purchased.");
        return;
    } else if (this.seats[seat].seatModel.getStatus() === STATUS_SELECTED) {
        // Seat is selected, free it
        ajaxFreeSeat(seat, function(data) {
            // Update model and view
            self.seats[seat].update(data[seat]);

            // Update counters
            self.counter.update(self.seats);
        });
    } else {
        // Seat is free or reserved, select it
        ajaxReserveSeat(seat, function(data) {
            // Update model and view
            self.seats[seat].update(data[seat]);
    
            // Update counters
            self.counter.update(self.seats);
        });
    }
};

Airplane.prototype.requestUpdate = function() {
    var self = this;

    // Retrieve the seat map
    ajaxGetSeatStatusAll(function(data) {
        // Update the seat models and views
        self.update(data);
    });
};

Airplane.prototype.requestPurchase = function() {
    var self = this;
    var toPurchase = [];

    // Check all seats to purchase
    for (var seat in this.seats) {
        if (this.seats[seat].seatModel.getStatus() === STATUS_SELECTED)
            toPurchase.push(this.seats[seat].seatModel.getSeatNum());
    }

    // Request the purchase
    ajaxPurchaseSeats(toPurchase, function(data) {
        // Update the seat models and views
        self.update(data);
    });
};