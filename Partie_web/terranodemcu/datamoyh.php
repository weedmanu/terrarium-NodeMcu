<?php

session_start();

$limit2 = $_SESSION['form']['limit']; 

if(empty($limit2))
		{
		  $limit2 = 1440;		    
		}

$limit = ($limit2 / 60);

require "connexion.php";
    
try {    
    $PDO = new PDO('mysql:host='.DB_HOST.';dbname='.DB_NAME, DB_USER, DB_PASS, array(PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8"));
    $PDO->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_WARNING); 
	
	$reponse = $PDO->query("SELECT DATE_FORMAT(date,'%d-%m-%Y %H') as moyheure FROM temphumi WHERE date < NOW() AND date > DATE_SUB(NOW(), INTERVAL $limit HOUR) GROUP BY DAY(date), HOUR(date)");	
	$rows = array();
	$rows['name'] = 'moyheure';
	while ($donnees = $reponse->fetch())
	{	
		$rows['data'][] = $donnees['moyheure'];		
	}	
	
	$reponse = $PDO->query('SELECT ROUND(AVG(TempC),2) as TempC FROM temphumi WHERE date < NOW() AND date > DATE_SUB(NOW(), INTERVAL '.$limit.' HOUR) GROUP BY DAY(date), HOUR(date)');	
	$rows1 = array();
	$rows1['name'] = 'TempC';
	while ($donnees = $reponse->fetch())
	{	
		$rows1['data'][] = $donnees['TempC'];		
	}
	
	$reponse = $PDO->query('SELECT ROUND(AVG(TempF),2) as TempF FROM temphumi WHERE date < NOW() AND date > DATE_SUB(NOW(), INTERVAL '.$limit.' HOUR) GROUP BY DAY(date), HOUR(date)');	
	$rows2 = array();
	$rows2['name'] = 'TempF';
	while ($donnees = $reponse->fetch())
	{	
		$rows2['data'][] = $donnees['TempF'];		
	}
	
	$reponse = $PDO->query('SELECT ROUND(AVG(HumiC),2) as HumiC FROM temphumi WHERE date < NOW() AND date > DATE_SUB(NOW(), INTERVAL '.$limit.' HOUR) GROUP BY DAY(date), HOUR(date)');	
	$rows3 = array();
	$rows3['name'] = 'HumiC';
	while ($donnees = $reponse->fetch())
	{	
		$rows3['data'][] = $donnees['HumiC'];		
	}
	
	$reponse = $PDO->query('SELECT ROUND(AVG(HumiF),2) as HumiF FROM temphumi WHERE date < NOW() AND date > DATE_SUB(NOW(), INTERVAL '.$limit.' HOUR) GROUP BY DAY(date), HOUR(date)');	
	$rows4 = array();
	$rows4['name'] = 'HumiF';
	while ($donnees = $reponse->fetch())
	{	
		$rows4['data'][] = $donnees['HumiF'];		
	}    
 
} catch(Exception $e) {
    echo "Impossible de se connecter à la base de données '".DB_NAME."' sur ".DB_HOST." avec le compte utilisateur '".DB_USER."'";
    echo "<br/>Erreur PDO : <i>".$e->getMessage()."</i>";
    die();
}


$result = array();
array_push($result,$rows);
array_push($result,$rows1);
array_push($result,$rows2);
array_push($result,$rows3);
array_push($result,$rows4);

// 6 - Afficher les résultats
print json_encode($result, JSON_NUMERIC_CHECK);

$reponse->closeCursor();

?>



