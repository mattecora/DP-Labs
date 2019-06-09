class CounterModel {
    constructor() {
        this.reset();
    }

    reset() {
        this.free = 0;
        this.reserved = 0;
        this.purchased = 0;
    }

    addFree() {
        this.free++;
    }

    addReserved() {
        this.reserved++;
    }

    addPurchased() {
        this.purchased++;
    }
}

class CounterView {
    constructor(counterModel) {
        this.counterModel = counterModel;
        this.render();
    }

    render() {
        $("#freeSeats").html(this.counterModel.free);
        $("#reservedSeats").html(this.counterModel.reserved);
        $("#purchasedSeats").html(this.counterModel.purchased);
    }
}

class CounterController {
    constructor() {
        this.counterModel = new CounterModel();
        this.counterView = new CounterView(this.counterModel);
    }

    update(seats) {
        this.counterModel.reset();

        // Check status of all seats
        for (let seat in seats) {
            if (seats[seat].seatModel.getStatus() === STATUS_FREE)
                this.counterModel.addFree();
            else if (seats[seat].seatModel.getStatus() === STATUS_RESERVED || seats[seat].seatModel.getStatus() === STATUS_SELECTED)
                this.counterModel.addReserved();
            else if (seats[seat].seatModel.getStatus() === STATUS_PURCHASED)
                this.counterModel.addPurchased();
        }

        // Update view
        this.counterView.render();
    }
}