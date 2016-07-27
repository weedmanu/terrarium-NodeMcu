Sur votre serveur :

Dans mysql ou autre bdd, créez une base de donnée , appelez la dht22.

Puis l' on crée la table suivante:

CREATE TABLE temphumi (date DATETIME, tempC DOUBLE, tempF DOUBLE, humiC DOUBLE, humiF DOUBLE);


Dans le dossier terranodemcu :

Il y a trois fichiers à modifier :

               bdd.php , data.php et dht22bdd.php 

Il faut juste remplacer LOGIN  et MOT DE PASSE par votre login et mdp de votre base de donnée.


Puis vous copier ce dossier (terranodemcu) à la racine de votre serveur web. (WWW)

Il faut également donner les droits en écriture au groupe html sur le fichier :

               data.csv
              

Pour voir votre page web : 

               http://adresse_ip_de_votre_serveur/terranodemcu
