class SeatView {
    constructor(seatModel, seatElement) {
        this.seatModel = seatModel;
        this.seatElement = seatElement;
        this.render();
    }

    render() {
        // Remove all classes
        this.seatElement.removeClass("free-seat reserved-seat purchased-seat selected-seat");
        
        // Show new seat status
        if (this.seatModel.getStatus() === STATUS_FREE)
            this.seatElement.addClass("free-seat");
        else if (this.seatModel.getStatus() === STATUS_RESERVED)
            this.seatElement.addClass("reserved-seat");
        else if (this.seatModel.getStatus() === STATUS_PURCHASED)
            this.seatElement.addClass("purchased-seat");
        else if (this.seatModel.getStatus() === STATUS_SELECTED)
            this.seatElement.addClass("selected-seat");
    }
}