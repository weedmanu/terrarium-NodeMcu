#! /usr/bin/env python
# -*- coding: utf-8 -*-

import urllib2
import os

hostname = "192.168.0.30" # terranodemcu
response = os.system("ping -c 1 " + hostname)

req = urllib2.Request("https://smsapi.free-mobile.fr/sendmsg?user=16211308&pass=bLdaXQbP4f6DRn&msg=Alerte%20!%20%20Connexion%20perdu%20!!!")

# et on controle la reponse
if response == 0:
	
  print hostname, 'is up!'
else:
  print hostname, 'is down!'
  url=urllib2.urlopen(req)
