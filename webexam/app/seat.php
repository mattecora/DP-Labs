<?php
    /*
        seat.php
        Provides the Seat class, used to represent the status of a seat
        Matteo Corain - Distributed programming I - A.Y. 2018-19
    */

    require_once "session.php";

    class Seat implements JsonSerializable {
        public const INVALID = null;
        public const FREE = 0;
        public const RESERVED = 1;
        public const PURCHASED = 2;
        public const SELECTED = 3;

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
            if (session_start_timeout() === SESSION_OK && $this->status === Seat::RESERVED && $_SESSION["username"] === $this->reserver)
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