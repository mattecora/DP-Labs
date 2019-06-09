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
            alert("Seat " + this.seats[seat].seatModel.getSeatNum() + " has already been purchased!");
            return;
        }
        
        // Check if the seat has been purchased in the meantime
        getSeatStatus(this.seats[seat].seatModel.getSeatNum(), data => {
            this.seats[seat].update(data[this.seats[seat].seatModel.getSeatNum()]);
            
            if (this.seats[seat].seatModel.getStatus() === STATUS_PURCHASED) {
                // Alert and update model and view
                alert("Seat " + this.seats[seat].seatModel.getSeatNum() + " has already been purchased!");
                
                // Update counters
                this.counter.update(this.seats);
                return;
            }
    
            // Check if the seat is selected or not
            if (this.seats[seat].seatModel.getStatus() === STATUS_SELECTED) {
                // Seat is selected and must be freed
                freeSeat(this.seats[seat].seatModel.getSeatNum(), data2 => {
                    // Alert and update model and view
                    this.seats[seat].update(STATUS_FREE);
                    alert("Seat " + this.seats[seat].seatModel.getSeatNum() + " freed!");

                    // Update counters
                    this.counter.update(this.seats);
                });
            } else {
                // Seat is free (or reserved by other) and must be selected
                reserveSeat(this.seats[seat].seatModel.getSeatNum(), data2 => {
                    // Alert and update model and view
                    this.seats[seat].update(STATUS_SELECTED);
                    alert("Seat " + this.seats[seat].seatModel.getSeatNum() + " selected!");

                    // Update counters
                    this.counter.update(this.seats);
                });
            }
        });
    }

    requestUpdate() {
        // Retrieve the seat map
        getSeatStatusAll(data => {
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
        purchaseSeats(toPurchase, data => {
            // Update the seat models and views
            this.update(data);
        });
    }
}