<?php
    /*
        seatmap.php
        Provides the SeatMap class, used to represent the cabin seat map
        Matteo Corain - Distributed programming I - A.Y. 2018-19
    */

    require_once "seat.php";

    class SeatMap implements JsonSerializable {
        const ROWS = 10;
        const PLACES = 6;
        
        private $seatmap;

        public function __construct() {
            $this->seatmap = array();

            // Create the seat map
            for ($row = 0; $row < SeatMap::ROWS; $row++) {
                for ($place = 0; $place < SeatMap::PLACES; $place++) {
                    $seat_num = SeatMap::generateSeatNum($row, $place);
                    $this->seatmap[$seat_num] = new Seat($seat_num, Seat::FREE, null);
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

        public static function generateSeatNum($row, $place) {
            // Check boundaries and generate seat number
            if ($row >= 0 && $row < SeatMap::ROWS && $place >= 0 && $place < SeatMap::PLACES)
                return chr($place + ord('A')). ($row + 1);
            return null;
        }

        public static function checkSeatNum($seat_num) {
            // Check seat format
            if (sscanf($seat_num, "%c%d", $place, $row) != 2)
                return null;
            
            // Transform to number formats
            $row = $row - 1;
            $place = ord($place) - ord('A');

            // Check equality
            return SeatMap::generateSeatNum($row, $place) === $seat_num;
        }
    }
?>