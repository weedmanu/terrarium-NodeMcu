 <?php
 
    $tempC = $_GET["tempC"];
    $humiC = $_GET["humiC"];
    $tempF = $_GET["tempF"];
    $humiF = $_GET["humiF"];
    
             
// Les lignes du tableau
$lignes[] = array('humi', 'temp');
$lignes[] = array($humiF, $tempF);
$lignes[] = array($humiC, $tempC);

// Paramétrage de l'écriture du futur fichier CSV
$chemin = 'data.csv';
$delimiteur = ','; // Pour une tabulation, utiliser $delimiteur = "t";

// Création du fichier csv (le fichier est vide pour le moment)
// w+ : consulter http://php.net/manual/fr/function.fopen.php
$fichier_csv = fopen($chemin, 'w+');

// Boucle foreach sur chaque ligne du tableau
foreach($lignes as $ligne){
	// chaque ligne en cours de lecture est insérée dans le fichier
	// les valeurs présentes dans chaque ligne seront séparées par $delimiteur
	fputcsv($fichier_csv, $ligne, $delimiteur);
}

// fermeture du fichier csv
fclose($fichier_csv);
			
?>
