class SeatMap {
    constructor(data) {
        this.seats = {};
        this.counter = new CounterController();

        for (let seat in data) {
            let seatModel = new SeatModel(seat, data[seat]);
            let seatView = new SeatView(seatModel, $("#" + seat));
            this.seats[seat] = new SeatController(seatModel, seatView);
        }

        this.counter.update(this.seats);
    }

    update(seats) {
        // Update seats
        for (let seat in this.seats)
            this.seats[seat].update(seats[seat]);
        
        // Update counters
        this.counter.update(this.seats);
    }

    requestSelect(seat) {
        // Check if the user is logged in
        if (username === undefined) {
            alert("Seat reservation is for registered users only!");
            return;
        }
    
        // Check if the seat is already purchased
        if (this.seats[seat].seatModel.getStatus() === STATUS_PURCHASED) {
            alert("Seat " + seat + " has already been purchased");
            return;
        }

        // Try to purchase the seat
        AJAX.reserveSeat(seat, data => {
            // Update model and view
            this.seats[seat].update(data[seat]);

            // Check the current status and alert accordingly
            if (this.seats[seat].seatModel.getStatus() === STATUS_SELECTED)
                alert("Seat " + this.seats[seat].seatModel.getSeatNum() + " selected!");
            else
            alert("Seat " + this.seats[seat].seatModel.getSeatNum() + " freed!");

            // Update counters
            this.counter.update(this.seats);
        });
    }

    requestUpdate() {
        // Retrieve the seat map
        AJAX.getSeatStatusAll(data => {
            // Update the seat models and views
            this.update(data);
        });
    }

    requestPurchase() {
        let toPurchase = [];

        // Check all seats to purchase
        for (let seat in this.seats) {
            if (this.seats[seat].seatModel.getStatus() === STATUS_SELECTED)
                toPurchase.push(this.seats[seat].seatModel.getSeatNum());
        }

        // Request the purchase
        AJAX.purchaseSeats(toPurchase, data => {
            // Update the seat models and views
            this.update(data);
        });
    }
}