 <?php
 
   $result = mysql_connect("localhost", "XXXXX", "yyyyy");  // changer par votre login et password de la BDD
   
    $tempC = $_GET["tempC"];
    $humiC = $_GET["humiC"];
    $tempF = $_GET["tempF"];
    $humiF = $_GET["humiF"];
    
    
$sqlt = "insert into dht22.temphumi (tempC,humiC,tempF,humiF) values ($tempC,$humiC,$tempF,$humiF)";
mysql_query($sqlt);


