<?php

class conexion{
   
    const user='xcom';
    const pass='xcom12345';
    const db='tesis';
    const servidor='localhost';

    public function conectardb(){
        $conectar = new mysqli(self::servidor, self::user,self::pass,self::db);
        if($conectar->connect_errno){
            die("Error en la coneccion".$conectar->connect_error);
        }
        return $conectar;
    }   
}

?>