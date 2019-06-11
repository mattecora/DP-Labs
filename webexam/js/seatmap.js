function SeatMap(data) {
    this.seats = {};
    this.counter = new CounterController();

    for (var seat in data) {
        var seatModel = new SeatModel(seat, data[seat]);
        var seatView = new SeatView(seatModel, $("#" + seat));
        this.seats[seat] = new SeatController(seatModel, seatView);
    }

    this.update = function(seats) {
        // Update seats
        for (var seat in this.seats)
            this.seats[seat].update(seats[seat]);
        
        // Update counters
        this.counter.update(this.seats);
    };

    this.counter.update(this.seats);

    this.requestSelect = function(seat) {
        var self = this;
    
        // Check if the seat is already purchased
        if (this.seats[seat].seatModel.getStatus() === STATUS_PURCHASED) {
            alert("Seat " + seat + " has already been purchased");
            return;
        }

        // Try to purchase the seat
        ajaxReserveSeat(seat, function(data) {
            // Update model and view
            self.seats[seat].update(data[seat]);

            // Update counters
            self.counter.update(self.seats);
        });
    };

    this.requestUpdate = function() {
        var self = this;

        // Retrieve the seat map
        ajaxGetSeatStatusAll(function(data) {
            // Update the seat models and views
            self.update(data);
        });
    };

    this.requestPurchase = function() {
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
}