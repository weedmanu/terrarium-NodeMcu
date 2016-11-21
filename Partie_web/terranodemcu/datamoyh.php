<?php
// 1 - Connexion à MySQL
$link = mysql_connect( 'localhost', 'XXXXX', 'YYYYY' ); //changer le login et password par celui de votre BDD.
if ( !$link ) {
  die( 'Could not connect: ' . mysql_error() );
}

// Sélection de la base de données
$db = mysql_select_db( 'dht22', $link );
if ( !$db ) {
  die ( 'Error selecting database temperatures : ' . mysql_error() );
}


$sth = mysql_query("SELECT DATE_FORMAT(date,'%d-%m-%Y %H') as moyheure FROM temphumi GROUP BY DAY(date), HOUR(date) ");
$rows = array();
$rows['name'] = 'moyheure';
while($r = mysql_fetch_array($sth)) {
    $rows['data'][] = $r['moyheure'];
}

//$sth = mysql_query("SELECT date, ROUND(AVG(tempC),2) as moytempC FROM temphumi WHERE date < NOW() AND date > DATE_SUB(NOW(), INTERVAL 600 HOUR) GROUP BY DAY(date), HOUR(date) ");
$sth = mysql_query("SELECT date, ROUND(AVG(tempC),2) as moytempC FROM temphumi GROUP BY DAY(date), HOUR(date) ");
$rows1 = array();
$rows1['name'] = 'moytempC';
while($rr = mysql_fetch_array($sth)) {
    $rows1['data'][] = $rr['moytempC'];
}

$sth = mysql_query("SELECT date, ROUND(AVG(tempF),2) as moytempF FROM temphumi  GROUP BY DAY(date), HOUR(date) ");
$rows2 = array();
$rows2['name'] = 'moytempF';
while($rrr = mysql_fetch_array($sth)) {
    $rows2['data'][] = $rrr['moytempF'];
}

$sth = mysql_query("SELECT date, ROUND(AVG(humiC),2) as moyhumiC FROM temphumi  GROUP BY DAY(date),  HOUR(date) ");
$rows3 = array();
$rows3['name'] = 'moyhumiC';
while($rrrr = mysql_fetch_array($sth)) {
    $rows3['data'][] = $rrrr['moyhumiC'];
}

$sth = mysql_query("SELECT date, ROUND(AVG(humiF),2) as moyhumiF FROM temphumi  GROUP BY DAY(date),  HOUR(date)");
$rows4 = array();
$rows4['name'] = 'moyhumiF';
while($rrrrr = mysql_fetch_array($sth)) {
    $rows4['data'][] = $rrrrr['moyhumiF'];
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



