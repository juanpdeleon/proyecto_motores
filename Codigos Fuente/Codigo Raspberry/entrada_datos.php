<?php
    require_once('conexion.php');
   
    $mac = $_POST['mac'];
    $AcX = $_POST['acx'];
	$AcY = $_POST['acy'];
	$AcZ = $_POST['acz'];
	$Tmp = $_POST['tmp'];
	
    $conn = new conexion();
    $query="INSERT INTO datos(Datos_Sensor,Datos_eje_x,Datos_eje_y,Datos_eje_z,Datos_tmp_s,Datos_fch_Hr) VALUES('$mac','$AcX','$AcY','$AcZ','$Tmp',CURRENT_TIMESTAMP);";
    $insert = mysqli_query($conn->conectardb(),$query);

?>