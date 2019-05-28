<?php
class Product {
    private $name, $desc, $price;

    public function __construct($name, $desc, $price) {
        $this->name = $name;
        $this->desc = $desc;
        $this->price = $price;
    }

    public function get_name() {
        return $this->name;
    }

    public function get_desc() {
        return $this->desc;
    }

    public function get_price() {
        return $this->price;
    }
}

$products = array(
    new Product("Coffee", "A good coffee", 1.50),
    new Product("Milk", "A quality milk", 1.20),
    new Product("Orange juice", "A perfect juice", 2.00),
    new Product("Coke", "A sugary coke", 1.80)
);
?>