 <?php
 
   $result = mysql_connect("localhost", "manu", "manu@.13"); 
   
    $tempC = $_GET["tempC"];
    $humiC = $_GET["humiC"];
    $tempF = $_GET["tempF"];
    $humiF = $_GET["humiF"];
    
    
$sqlt = "insert into dht22.temphumi (tempC,humiC,tempF,humiF) values ($tempC,$humiC,$tempF,$humiF)";
mysql_query($sqlt);

if ($tempC < 20) {	
	fopen("https://smsapi.free-mobile.fr/sendmsg?user=16211308&pass=bLdaXQbP4f6DRn&msg=Alerte%20!%20Limit%20atteinte%20:%20tempC%20=%20$tempC%20°C%20,%20tempF%20=%20$tempF%20°C%20,%20humiC%20=%20$humiC%20%%20,%20humiF%20=%20$humiF%%20", "r");
}
		
?>
