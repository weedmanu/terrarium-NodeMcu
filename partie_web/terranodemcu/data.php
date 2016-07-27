<?php
// 1 - Connexion à MySQL
$link = mysql_connect( 'localhost', 'LOGIN', 'MOT DE PASSE' ); //changer password par votre mot de passe si vous l'avez changé.
if ( !$link ) {
  die( 'Could not connect: ' . mysql_error() );
}

// Sélection de la base de données
$db = mysql_select_db( 'dht22', $link );
if ( !$db ) {
  die ( 'Error selecting database temperatures : ' . mysql_error() );
}

$sth = mysql_query("SELECT date FROM temphumi"); 
$rows = array();
$rows['name'] = 'date';
while($r = mysql_fetch_array($sth)) {
    $rows['data'][] = $r['date'];
}

$sth = mysql_query("SELECT tempC FROM temphumi ");
$rows1 = array();
$rows1['name'] = 'tempC';
while($rr = mysql_fetch_array($sth)) {
    $rows1['data'][] = $rr['tempC'];
}

$sth = mysql_query("SELECT tempF FROM temphumi ");
$rows2 = array();
$rows2['name'] = 'tempF';
while($rrr = mysql_fetch_array($sth)) {
    $rows2['data'][] = $rrr['tempF'];
}

$sth = mysql_query("SELECT humiC FROM temphumi ");
$rows3 = array();
$rows3['name'] = 'humiC';
while($rrrr = mysql_fetch_array($sth)) {
    $rows3['data'][] = $rrrr['humiC'];
}

$sth = mysql_query("SELECT humiF FROM temphumi ");
$rows4 = array();
$rows4['name'] = 'humiF';
while($rrrrr = mysql_fetch_array($sth)) {
    $rows4['data'][] = $rrrrr['humiF'];
}



$result = array();
array_push($result,$rows);
array_push($result,$rows1);
array_push($result,$rows2);
array_push($result,$rows3);
array_push($result,$rows4);


// 6 - Afficher les résultats
print json_encode($result, JSON_NUMERIC_CHECK);


// Fermer la connexion à MySQL
mysql_close($link);
?>

