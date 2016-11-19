#! /usr/bin/env python
# -*- coding: utf-8 -*-

import urllib2
import os

hostname = "192.168.XXX.XXX" # terranodemcu
response = os.system("ping -c 1 " + hostname)

#commande pour l' envoi d'un sms ( fonctionne uniquement chez free mobile option gratuite)
req = urllib2.Request("https://smsapi.free-mobile.fr/sendmsg?user=XXXXXXXX&pass=YYYYYYYYY&msg=Alerte%20!%20%20Connexion%20perdu%20!!!")

# et on controle la reponse
if response == 0:	
  print hostname, 'is up!'
else:
  print hostname, 'is down!'
  url=urllib2.urlopen(req) # on envoi la commande si la com est perdu
  
# programme à lancer en tache toutes les heures sur le serveur .
