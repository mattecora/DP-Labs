/**
    counter.js
    Provides the classes used to implement the counters, organized according to the MVC pattern
    Matteo Corain - Distributed programming I - A.Y. 2018-19
*/

function CounterModel() {
    this.free = 0;
    this.reserved = 0;
    this.purchased = 0;

    this.reset = function() {
        this.free = 0;
        this.reserved = 0;
        this.purchased = 0;
    };

    this.addFree = function() {
        this.free++;
    };

    this.addReserved = function() {
        this.reserved++;
    };

    this.addPurchased = function() {
        this.purchased++;
    };
}

function CounterView(counterModel, freeSeatsElement, reservedSeatsElement, purchasedSeatsElement) {
    this.counterModel = counterModel;
    this.freeSeatsElement = freeSeatsElement;
    this.reservedSeatsElement = reservedSeatsElement;
    this.purchasedSeatsElement = purchasedSeatsElement;

    this.render = function() {
        this.freeSeatsElement.html(this.counterModel.free);
        this.reservedSeatsElement.html(this.counterModel.reserved);
        this.purchasedSeatsElement.html(this.counterModel.purchased);
    };

    this.render();
}

function CounterController(counterModel, counterView) {
    this.counterModel = counterModel;
    this.counterView = counterView;

    this.update = function(seats) {
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
}