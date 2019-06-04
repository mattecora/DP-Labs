class SeatMap {
    constructor(seats) {
        this.seatControllers = {};
        this.counter = new CounterController();

        for (let seat in seats) {
            let seatModel = new SeatModel(seat, seats[seat]);
            let seatView = new SeatView(seatModel, $("#" + seat));
            this.seatControllers[seat] = new SeatController(seatModel, seatView);
        }

        this.counter.update(seats);
    }

    update(seats) {
        // Update seats
        for (let seat in this.seatControllers)
            this.seatControllers[seat].update(seats[seat]);
        
        // Update counters
        this.counter.update(seats);
    }

    requestSelect(seat) {
        // Request the seat
        this.seatControllers[seat].requestSelect();
    }

    requestUpdate() {
        // Retrieve the seat map
        getSeatStatusAll((data, status, xhr) => {
            // Update the seat models and views
            this.update(data);
        });
    }

    requestPurchase() {
        let toPurchase = [];

        // Check all seats to purchase
        for (let seat in this.seatControllers) {
            if (this.seatControllers[seat].seatModel.getStatus() === STATUS_SELECTED)
                toPurchase.push(this.seatControllers[seat].seatModel.getSeatNum());
        }

        console.log(toPurchase);

        // Request the purchase
        purchaseSeats(toPurchase, (data, status, xhr) => {
            // Update the seat models and views
            this.update(data);
        });
    }
}