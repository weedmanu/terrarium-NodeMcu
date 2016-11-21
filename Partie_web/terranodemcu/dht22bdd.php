 <?php
 
 require "connexion.php";   
   
    $tempC = $_GET["tempC"];
    $humiC = $_GET["humiC"];
    $tempF = $_GET["tempF"];
    $humiF = $_GET["humiF"];
    
    
$sqlt = "insert into temphumi (tempC,humiC,tempF,humiF) values ($tempC,$humiC,$tempF,$humiF)";
mysql_query($sqlt);


?>
