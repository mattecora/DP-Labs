<?php
    /*
        message.php
        Provides the Message class, describing a message to be returned to the user
        Matteo Corain - Distributed programming I - A.Y. 2018-19
    */

    class Message implements JsonSerializable {
        private $success;
        private $message;
        private $data;

        public function __construct($success, $message, $data) {
            $this->success = $success;
            $this->message = $message;
            $this->data = $data;
        }

        public function getSuccess() {
            return $this->success;
        }

        public function getMessage() {
            return $this->message;
        }

        public function getData() {
            return $this->data;
        }

        public function jsonSerialize() {
            return array("success" => $this->success, "message" => $this->message, "data" => $this->data);
        }
    }
?>