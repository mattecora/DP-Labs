<?php
    class Seat implements JsonSerializable {
        public const INVALID = null;
        public const FREE = 0;
        public const RESERVED = 1;
        public const PURCHASED = 2;
        
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
            return array($this->seat_num => array("status" => $this->status, "reserver" => $this->reserver));
        }
    }
?>