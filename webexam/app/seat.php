<?php
    /*
        seat.php
        Provides the Seat class, used to represent the status of a seat
        Matteo Corain - Distributed programming I - A.Y. 2018-19
    */

    require_once "session.php";

    class Seat implements JsonSerializable {
        const INVALID = null;
        const FREE = 0;
        const RESERVED = 1;
        const PURCHASED = 2;
        const SELECTED = 3;

        private $seat_num;
        private $status;
        private $reserver;

        public function __construct($seat_num, $status, $reserver) {
            $this->seat_num = $seat_num;
            $this->status = $status;
            $this->reserver = $reserver;
        }

        public function getSeatNum() {
            return $this->seat_num;
        }
        
        public function getStatus() {
            $session = Session::get(true);
            
            // If the seat has been reserved by the logged user, return Seat::SELECTED
            if ($session->getStatus() === Session::STATUS_OK && $this->status === Seat::RESERVED && $session->getUsername() === $this->reserver)
                return Seat::SELECTED;
            return $this->status;
        }

        public function getReserver() {
            return $this->reserver;
        }

        public function setStatus($status) {
            $this->status = $status;
        }

        public function setReserver($reserver) {
            $this->reserver = $reserver;
        }

        public function jsonSerialize() {
            return array($this->seat_num => $this->getStatus());
        }
    }
?>