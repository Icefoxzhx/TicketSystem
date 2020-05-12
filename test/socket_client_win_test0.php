<?php  
class Byte{  
    //长度   
    private $length=0;  
      
    private $byte='';  
    //操作码   
    private $code;  
    public function setBytePrev($content){  
        $this->byte=$content.$this->byte;  
    }  
    public function getByte(){  
        return $this->byte;  
    }  
    public function getLength(){  
        return $this->length;  
    }  
    public function writeChar($string){  
        $this->length+=strlen($string);  
        $str=array_map('ord',str_split($string));  
        foreach($str as $vo){  
            $this->byte.=pack('c',$vo);  
        }  
        $this->byte.=pack('c','0');  
        $this->length++;  
    }  
    public function writeInt($str){  
        $this->length+=4;  
        $this->byte.=pack('L',$str);  
    }  
    public function writeShortInt($interge){  
        $this->length+=2;  
        $this->byte.=pack('v',$interge);  
    }  
}  
class GameSocket{  
    private $socket;  
    private $port=9991;  
    private $host='127.0.0.1';  
    private $byte;  
    private $code;  
    const CODE_LENGTH=2;  
    const FLAG_LENGTH=4;  
    public function __set($name,$value){  
        $this->$name=$value;  
    }  
    public function __construct($host='127.0.0.1',$port=9991){  
        $this->host=$host;  
        $this->port=$port;  
        $this->socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);  
        if(!$this->socket){  
            exit('创建socket失败');  
        }  
        $result = socket_connect($this->socket,$this->host,$this->port);  
        if(!$result){  
            exit('连接不上目标主机'.$this->host);  
        }  
        $this->byte=new Byte();  
    }  
    public function write($data){  
        if(is_string($data)||is_int($data)||is_float($data)){  
            $data[]=$data;  
        }  
        if(is_array($data)){  
            foreach($data as $vo){  
                $this->byte->writeShortInt(strlen($vo));  
                $this->byte->writeChar($vo);  
            }  
        }  
        $this->setPrev();  
        $this->send();  
    }  
    /* 
     *设置表头部分 
     *表头=length+code+flag 
     *length是总长度(4字节)  code操作标志(2字节)  flag暂时无用(4字节) 
     */  
    private function getHeader(){  
        $length=$this->byte->getLength();  
        $length=intval($length)+self::CODE_LENGTH+self::FLAG_LENGTH;  
        return pack('L',$length);  
    }  
    private function getCode(){  
        return pack('v',$this->code);  
    }  
    private function getFlag(){  
        return pack('L',24);  
    }  
      
    private function setPrev(){  
        $this->byte->setBytePrev($this->getHeader().$this->getCode().$this->getFlag());  
    }  
  
    private function send(){  
        $result=socket_write($this->socket,$this->byte->getByte());  
        if(!$result){  
            exit('发送信息失败');  
        }  
    }  
    public function __desctruct(){  
        socket_close($this->socket);  
    }  
}  
  
$data[]='testzouhao';  
$data[]='a';  
$gameSocket=new GameSocket();  
$gameSocket->code=11;  
$gameSocket->write($data);  

?>
