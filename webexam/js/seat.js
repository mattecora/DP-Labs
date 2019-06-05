const STATUS_FREE = 0;
const STATUS_RESERVED = 1;
const STATUS_PURCHASED = 2;
const STATUS_SELECTED = 3;

class SeatModel {
    constructor(seatNum, status) {
        this.seatNum = seatNum;
        this.status = status;
    }

    getSeatNum() {
        return this.seatNum;
    }

    getStatus() {
        return this.status;
    }

    setSeatNum(seatNum) {
        this.seatNum = seatNum;
    }

    setStatus(status) {
        this.status = status;
    }
}

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
}