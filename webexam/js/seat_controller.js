class SeatController {
    constructor (seatModel, seatView) {
        this.seatModel = seatModel;
        this.seatView = seatView;
    }

    update(status) {
        // Update the model and render the view
        this.seatModel.setStatus(status);
        this.seatView.render(status);
    }

    requestSelect() {
        // Check if the user is logged in
        if (username === undefined) {
            alert("Seat reservation is for registered users only!");
            return;
        }
    
        // Check if the seat is already purchased
        if (this.seatModel.getStatus() === STATUS_PURCHASED) {
            alert("Seat " + this.seatModel.getSeatNum() + " has already been purchased!");
            return;
        }
        
        // Check if the seat has been purchased in the meantime
        getSeatStatus(this.seatModel.getSeatNum(), (data, status, xhr) => {
            this.update(data[this.seatModel.getSeatNum()]);
            
            if (this.seatModel.getStatus() === STATUS_PURCHASED) {
                // Alert and update model and view
                alert("Seat " + this.seatModel.getSeatNum() + " has already been purchased!");
                return;
            }
    
            // Check if the seat is selected or not
            if (this.seatModel.getStatus() === STATUS_SELECTED) {
                // Seat is selected and must be freed
                freeSeat(this.seatModel.getSeatNum(), (data2, status2, xhr2) => {
                    // Alert and update model and view
                    this.update(STATUS_FREE);
                    alert("Seat " + this.seatModel.getSeatNum() + " freed!");
                });
            } else {
                // Seat is free (or reserved by other) and must be selected
                reserveSeat(this.seatModel.getSeatNum(), (data2, status2, xhr2) => {
                    // Alert and update model and view
                    this.update(STATUS_SELECTED);
                    alert("Seat " + this.seatModel.getSeatNum() + " selected!");
                });
            }
        });
    }
}