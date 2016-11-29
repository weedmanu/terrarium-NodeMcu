<?php

session_start();

$limit = $_SESSION['form']['limit']; 

if(empty($_SESSION['form']['limit']))
	{
	  $limit = 1440;	//(60min * 24 hr)
	}

require "connexion.php";
    
try {    
    $PDO = new PDO('mysql:host='.DB_HOST.';dbname='.DB_NAME, DB_USER, DB_PASS, array(PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8"));
    $PDO->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_WARNING);  
    
    $sql = 'SELECT COUNT(*) AS nb FROM temphumi';
    $result = $PDO->query($sql);
    $columns = $result->fetch();
    $nb = $columns['nb'];
    
    $ligne = $nb - $limit;

	if ($ligne < 0) 
	{
		$ligne = 0;
	}
       
    $ligne =(int)$ligne;
    $nb =(int)$nb;
    
	$reponse = $PDO->prepare('SELECT * FROM temphumi LIMIT :ligne, :nb');
	$reponse->bindValue('ligne', $ligne, PDO::PARAM_INT);
	$reponse->bindValue('nb', $nb, PDO::PARAM_INT);
	$reponse->execute();
	$rows = array();
	$rows['name'] = 'date';
	while ($valeur = $reponse->fetch())
	{	
		$rows['data'][] = $valeur['date'];		
	}

	
	$reponse = $PDO->prepare('SELECT TempC FROM temphumi LIMIT :ligne, :nb');
	$reponse->bindValue('ligne', $ligne, PDO::PARAM_INT);
	$reponse->bindValue('nb', $nb, PDO::PARAM_INT);
	$reponse->execute();	
	$rows1 = array();
	$rows1['name'] = 'TempC';
	while ($donnees = $reponse->fetch())
	{	
		$rows1['data'][] = $donnees['TempC'];		
	}
	
	$reponse = $PDO->prepare('SELECT TempF FROM temphumi LIMIT :ligne, :nb');
	$reponse->bindValue('ligne', $ligne, PDO::PARAM_INT);
	$reponse->bindValue('nb', $nb, PDO::PARAM_INT);
	$reponse->execute();
	$rows2 = array();
	$rows2['name'] = 'TempF';
	while ($donnees = $reponse->fetch())
	{	
		$rows2['data'][] = $donnees['TempF'];		
	}
	
	$reponse = $PDO->prepare('SELECT HumiC FROM temphumi LIMIT :ligne, :nb');
	$reponse->bindValue('ligne', $ligne, PDO::PARAM_INT);
	$reponse->bindValue('nb', $nb, PDO::PARAM_INT);
	$reponse->execute();	
	$rows3 = array();
	$rows3['name'] = 'HumiC';
	while ($donnees = $reponse->fetch())
	{	
		$rows3['data'][] = $donnees['HumiC'];		
	}
	
	$reponse = $PDO->prepare('SELECT HumiF FROM temphumi LIMIT :ligne, :nb');
	$reponse->bindValue('ligne', $ligne, PDO::PARAM_INT);
	$reponse->bindValue('nb', $nb, PDO::PARAM_INT);
	$reponse->execute();	
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
