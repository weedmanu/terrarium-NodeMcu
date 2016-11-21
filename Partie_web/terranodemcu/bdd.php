<?php

require "connexion.php";

// 2 - Récupération du nombre de lignes contenu dans la table
$rqut_nb ="SELECT COUNT( date ) as recuperation FROM temphumi ;";
$rslt_nb = mysql_query( $rqut_nb) or die('Erreur SQL !<br />'.$sql.'<br />'.mysql_error());  
$data_nb = mysql_fetch_array($rslt_nb);
$nb = ''.$data_nb['recuperation'].'';
$ef = $nb - 672;              // ne garde que 672 entrées si on purge , a modifier selon vos besoin ici une semaine.

echo "il y a $nb entrées dans la base de donnée.";

if($nb > 1344) //Si le nombre d'entrée est > a 1344 ici 2 semaines.
     {
          echo '<br>';
          echo 'Veux tu purger la base de donnée ? ';
          echo '<br>';
          echo '
<form method="post" action="index.php">
<input type="radio" name="reponse" value="oui">
Oui
<input type="radio" name="reponse" value="non">
Non
<input type="submit" value="Valider">
</form> </br>';

     }

    $reponse=$_POST['reponse'];    

	if($reponse=="oui") {
		
			// lancement de la requête pour effacer 
		$sql ="DELETE from temphumi ORDER BY date ASC LIMIT $ef";
		// on exécute la requête (mysql_query) et on affiche un message au cas où la requête ne se passait pas bien (or die)
		mysql_query($sql) or die('Erreur SQL !'.$sql.'<br />'.mysql_error());
		echo '</br>';
		echo "ok effacé , il ne reste que 7 jours de data";
		echo '</br>';
		
		}
			
		elseif($reponse=="non") {
			echo '</br>';
			echo "OK , on efface rien ";			
			echo '</br>';
			} 
			else {
				echo"";
				}

// Fermer la connexion à MySQL
mysql_close($link);
?>
