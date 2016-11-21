<?php
// 1 - Connexion à MySQL
$link = mysql_connect( 'localhost', 'XXXX', 'YYYY' ); // changer par votre login et password de votre base de donnée.
if ( !$link ) {
  die( 'Could not connect: ' . mysql_error() );
}

// Sélection de la base de données
$db = mysql_select_db( 'dht22', $link );
if ( !$db ) {
  die ( 'Error selecting database temperatures : ' . mysql_error() );
    
}
