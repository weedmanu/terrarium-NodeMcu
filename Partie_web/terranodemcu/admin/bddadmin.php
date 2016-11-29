<?php

require "../connexion.php";
    
try {    
    $PDO = new PDO('mysql:host='.DB_HOST.';dbname='.DB_NAME, DB_USER, DB_PASS, array(PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8"));
    $PDO->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_WARNING); 
 
    $sql = 'SELECT COUNT(*) AS nb FROM temphumi';
    $result = $PDO->query($sql);
    $columns = $result->fetch();
    $nb = $columns['nb'];
    echo '<br/>';
    echo 'il y a '.$nb.' entrées dans la base de donnée';
	echo '<br/>';
	echo '<p>	
		Veux tu la sauvegarder ? 
		<form method="post" action="index.php">
		<input type="hidden" name="reponse" value="oui">
		<p><img src="../img/serpent3.gif" alt="gif"/></p>
		<input type="submit" value="Valider">
		</form> 
		</p>				
		';     

    $reponse=$_POST['reponse'];    

	if($reponse=="oui") {		
		include "backup.php";		
		}	
		
} catch(Exception $e) {
    echo "Impossible de se connecter à la base de données '".DB_NAME."' sur ".DB_HOST." avec le compte utilisateur '".DB_USER."'";
    echo "<br/>Erreur PDO : <i>".$e->getMessage()."</i>";
    die();
}

$result->closeCursor();
?>
