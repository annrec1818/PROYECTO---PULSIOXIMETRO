<?php

include'conexion.php';

if ($con) {
    echo "Conexion con base de datos exitosa! ";
    
    if(isset($_POST['pulso'])) {
        $pulso = $_POST['pulso'];
        echo " pulso : ".$pulso;
    }
 
    if(isset($_POST['oxigenacion'])) { 
        $oxigenacion = $_POST['oxigenacion'];
        echo " oxigenacion : ".$oxigenacion;
        date_default_timezone_set('america/bogota');
        $fecha_actual = date("Y-m-d H:i:s");
        
        $consulta = "INSERT INTO Tb_PULSOX(pulso, oxigenacion, fecha_actual) VALUES ('$pulso','$oxigenacion', '$fecha_actual')";
       // $consulta = "UPDATE Tb_PULSOX pulso='$pulso',oxigenacion='$oxigenacion' WHERE Id = 1";
        $resultado = mysqli_query($con, $consulta);
        if ($resultado){
            echo " Registo en base de datos OK! ";
        } else {
            echo " Falla! Registro BD";
        }
    }
    
    
} else {
    echo "Falla! conexion con Base de datos ";   
}


?>