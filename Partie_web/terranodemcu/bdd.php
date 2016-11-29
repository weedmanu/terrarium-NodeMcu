<?php

require "connexion.php";
    
try {    
    $PDO = new PDO('mysql:host='.DB_HOST.';dbname='.DB_NAME, DB_USER, DB_PASS, array(PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8"));
    $PDO->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_WARNING);
 
    $sql = 'SELECT COUNT(*) AS nb FROM temphumi';
    $result = $PDO->query($sql);
    $columns = $result->fetch();
    $nb = $columns['nb'];
     
   echo 'il y a '.$nb.' entrées dans la base de donnée';
   
 
} catch(Exception $e) {
    echo "Impossible de se connecter à la base de données '".DB_NAME."' sur ".DB_HOST." avec le compte utilisateur '".DB_USER."'";
    echo "<br/>Erreur PDO : <i>".$e->getMessage()."</i>";
    die();
}	

$result->closeCursor();
?>
