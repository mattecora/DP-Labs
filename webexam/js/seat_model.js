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