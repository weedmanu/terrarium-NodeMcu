 <?php 
  
$TempC = $_GET["tempC"];
$HumiC = $_GET["humiC"];
$TempF = $_GET["tempF"];
$HumiF = $_GET["humiF"];

require "connexion.php";
    
try {    
    $PDO = new PDO('mysql:host='.DB_HOST.';dbname='.DB_NAME, DB_USER, DB_PASS, array(PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8"));
    $PDO->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_WARNING);    

	$stmt = $PDO->prepare("INSERT INTO temphumi (tempC, humiC, tempF, humiF) values (:tempC, :humiC, :tempF, :humiF)");
	$stmt->bindParam('tempC', $TempC);
	$stmt->bindParam('humiC', $HumiC);
	$stmt->bindParam('tempF', $TempF);
	$stmt->bindParam('humiF', $HumiF);
	$stmt->execute();
				
} catch(Exception $e) {
    echo "Impossible de se connecter à la base de données '".DB_NAME."' sur ".DB_HOST." avec le compte utilisateur '".DB_USER."'";
    echo "<br/>Erreur PDO : <i>".$e->getMessage()."</i>";
    die();
}				

$stmt->closeCursor();

        
// Les lignes du tableau
$lignes[] = array('humi', 'temp');
$lignes[] = array($HumiF, $TempF);
$lignes[] = array($HumiC, $TempC);

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
