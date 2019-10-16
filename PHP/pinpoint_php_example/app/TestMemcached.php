<?php


namespace app;


class TestMemcached
{
    protected $mc;

    public function mem_connect(){
        $this->mc = new memcached();
        $this->mc->addServer("10.34.130.152", "11211");
    }

    public function mem_add($key, $value){
        if($this->mc->add($key, $value)){
            return $this->mc->get($key);
        }else{
            return false;
        }

    }

    public function mem_get($key){
        $val = $this->mc->get($key);
        return $val;
    }

    public function mem_replace($key, $value){
        $val = $this->mc->get($key);
        if($this->mc->replace($key, $value)){
            echo $this->mc->get($key);
            return $val;
        }else{
            return false;
        }
    }

    public function mem_del($key){
        $this->mc->delete($key);
    }
}