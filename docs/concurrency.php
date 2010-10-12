<?php
//orginal model
class Concurrency {
    public $requests    = array();
    public $responses   = array();

    public $headers     = array();
    
    
    public function request(){
    }

    public function addRequest(ConcurrencyRequest $request) {
    }
}

class ConcurrencyRequest {
    public $url;
}
