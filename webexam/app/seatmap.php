<?php
    class SeatMap implements JsonSerializable {
        public const ROWS = 10;
        public const PLACES = 6;

        private $seatmap;

        public function __construct() {
            $this->seatmap = array();

            // Create the seat map
            for ($row = 0; $row < SeatMap::ROWS; $row++) {
                for ($place = 1; $place <= SeatMap::PLACES; $place++) {
                    $this->seatmap[chr($row + ord('A')) . $place] = new Seat(chr($row + ord('A')) . $place, Seat::FREE, null);
                }
            }
        }

        public function getSeat($seat_num) {
            if (isset($this->seatmap[$seat_num]))
                return $this->seatmap[$seat_num];
            return new Seat($seat_num, Seat::INVALID, null);
        }

        public function updateSeat($seat) {
            // If the seat is not in the seat map, do nothing
            if (!isset($this->seatmap[$seat->getSeatNum()]))
                return;
            
            // Update the seat status
            $this->seatmap[$seat->getSeatNum()]->setStatus($seat->getStatus());
            $this->seatmap[$seat->getSeatNum()]->setReserver($seat->getReserver());
        }

        public function jsonSerialize() {
            return array_map(function ($data) {
                return $data->getStatus();
            }, $this->seatmap);
        }
    }
?>