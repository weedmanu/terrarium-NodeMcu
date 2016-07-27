 <?php
   
   // on récup les variables GET envoyer par le NodeMcu , et on les déclare en variable PHP.
   
    $tempC = $_GET["tempC"];
    $humiC = $_GET["humiC"];
    $tempF = $_GET["tempF"];
    $humiF = $_GET["humiF"];
    
    $result = mysql_connect("localhost", "LOGIN", "MOT DE PASS"); // remplacer par votre login et mot de passe de la base de donnée 
    
	$sqlt = "insert into dht22.temphumi (tempC,humiC,tempF,humiF) values ($tempC,$humiC,$tempF,$humiF)";  // on prépare les datas
	
	mysql_query($sqlt); // on insére les datas dans la bdd

?>

