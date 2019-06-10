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

function CounterView(counterModel) {
    this.counterModel = counterModel;

    this.render = function() {
        $("#freeSeats").html(this.counterModel.free);
        $("#reservedSeats").html(this.counterModel.reserved);
        $("#purchasedSeats").html(this.counterModel.purchased);
    };

    this.render();
}

function CounterController() {
    this.counterModel = new CounterModel();
    this.counterView = new CounterView(this.counterModel);

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