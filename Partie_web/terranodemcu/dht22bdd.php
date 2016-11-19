 <?php
 
   $result = mysql_connect("localhost", "XXXX", "YYYY"); // connexion à la base de donnée
   
   // récupération des variable GET html et on les passe en variable php
    $tempC = $_GET["tempC"];
    $humiC = $_GET["humiC"];
    $tempF = $_GET["tempF"];
    $humiF = $_GET["humiF"];
    
    // on insert les datas dans la base de donnée
$sqlt = "insert into dht22.temphumi (tempC,humiC,tempF,humiF) values ($tempC,$humiC,$tempF,$humiF)";
mysql_query($sqlt);

<<<<<<< HEAD
//on ferme la connexion à MySQL
mysql_close($result);

// envoi d'un sms ( fonctionne uniquement chez free mobile option gratuite)
if ($tempC < 20) {	
	fopen("https://smsapi.free-mobile.fr/sendmsg?user=XXXXXXXX&pass=YYYYYYYYY&msg=Alerte%20!%20Limit%20atteinte%20:%20tempC%20=%20$tempC%20°C%20,%20tempF%20=%20$tempF%20°C%20,%20humiC%20=%20$humiC%20%%20,%20humiF%20=%20$humiF%%20", "r");
}
=======
>>>>>>> 5be25c3c4a493151bd5fef4af9fff28bdcbe1873
		
?>
