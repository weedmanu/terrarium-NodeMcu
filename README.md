Gestion d'un terrarium par un nodemcu , avec l'IDE Arduino.

un bouton poussoir , 2 sondes DHT22 , un module 2 relais , un ecran lcd 16x2 avec un convertiseur I2c et un nodemcu pour la  gestion du chauffage et de la lumière .

En plus de la gestion :

- les datas sont affiche sur l'ecran lcd si l'on appuie sur le bouton

- les datas sont envoyé sur un serveur web distant afin de créer une page web affichant les datas.

Le programme se nomme terranodemcu.ino , remplacez dans le programme , votre SSID , le mot de passe associé et l'adresse IP de votre serveur web.

Dans le programme terranodemcu.ino l'adresse I2C par defaut est 03xF (ligne 10) , le programme scanI2C sert a trouver votre adresse s'il elle diffère du programme.

un tutoriel se trouve ici :

    
     https://freemanbox.freeboxos.fr/tuto-terranodemcu/
     
     ou ici:
     
     https://docs.google.com/presentation/d/1g7XbasA5Uc-QHqzkxNnwfHUlCG-mjSonYLsY-LYzw_s/edit?usp=sharing
     
