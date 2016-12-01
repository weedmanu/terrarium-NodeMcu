#!/bin/bash
# remplacez XXXX et YYYY par votre login et mot de pass de la bdd
mysql -uXXXXX -pYYYYY -Ddht22 -e "SELECT ROUND(AVG(tempC),0) as moytempC FROM temphumi GROUP BY DAY(date) DESC, HOUR(date) DESC LIMIT 1;" >> test.txt
tempC=`sed -n '$p' test.txt`
rm test.txt
# fonctionne uniquement si vous êtes chez free mobile, activer l'option de votre ligne et renseigné votre user et mdp a la place des XXXX et YYYY 
if  (( $tempC < 22 )) ; then	
	wget "https://smsapi.free-mobile.fr/sendmsg?user=XXXXXX&pass=YYYYYY&msg=Alerte%20!%20Limite%20atteinte%20:%20tempC%20=%20$tempC%20°C%20"
fi
#  l'ip de votre nodemxcu:
while ! ping -c5 192.168.XXX.XXX
        do wget "https://smsapi.free-mobile.fr/sendmsg?user=XXXXXX&pass=YYYYYY&msg=Alerte%20!%20Connexion%20perdue%20!!!"
done
exit
