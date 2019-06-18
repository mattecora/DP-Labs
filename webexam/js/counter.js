/**
    counter.js
    Provides the classes used to implement the counters, organized according to the MVC pattern
    Matteo Corain - Distributed programming I - A.Y. 2018-19
*/

// Counter model

function CounterModel() {
    this.free = 0;
    this.reserved = 0;
    this.purchased = 0;
}

CounterModel.prototype.reset = function() {
    this.free = 0;
    this.reserved = 0;
    this.purchased = 0;
};

CounterModel.prototype.addFree = function() {
    this.free++;
};

CounterModel.prototype.addReserved = function() {
    this.reserved++;
};

CounterModel.prototype.addPurchased = function() {
    this.purchased++;
};

// Counter view

function CounterView(counterModel, freeSeatsElement, reservedSeatsElement, purchasedSeatsElement) {
    this.counterModel = counterModel;
    this.freeSeatsElement = freeSeatsElement;
    this.reservedSeatsElement = reservedSeatsElement;
    this.purchasedSeatsElement = purchasedSeatsElement;

    this.render();
}

CounterView.prototype.render = function() {
    this.freeSeatsElement.html(this.counterModel.free);
    this.reservedSeatsElement.html(this.counterModel.reserved);
    this.purchasedSeatsElement.html(this.counterModel.purchased);
};

// Counter controller

function CounterController(counterModel, counterView) {
    this.counterModel = counterModel;
    this.counterView = counterView;
}

CounterController.prototype.update = function(seats) {
    this.counterModel.reset();

    // Check status of all seats
    for (var seat in seats) {
        if (seats[seat].seatModel.getStatus() === STATUS_FREE)
            this.counterModel.addFree();
        else if (seats[seat].seatModel.getStatus() === STATUS_RESERVED || seats[seat].seatModel.getStatus() === STATUS_SELECTED)
            this.counterModel.addReserved();
        else if (seats[seat].seatModel.getStatus() === STATUS_PURCHASED)
            this.counterModel.addPurchased();
    }

    // Update view
    this.counterView.render();
};