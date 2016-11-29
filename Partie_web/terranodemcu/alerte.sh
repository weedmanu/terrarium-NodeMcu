#!/bin/bash
mysql -umanu -pmanu@.13 -Ddht22 -e "SELECT ROUND(AVG(tempC),0) as moytempC FROM temphumi GROUP BY DAY(date) DESC, HOUR(date) DESC LIMIT 1;" >> test.txt
tempC=`sed -n '$p' test.txt`
rm test.txt
if  (( $tempC < 22 )) ; then	
	wget "https://smsapi.free-mobile.fr/sendmsg?user=16211308&pass=bLdaXQbP4f6DRn&msg=Alerte%20!%20Limite%20atteinte%20:%20tempC%20=%20$tempC%20°C%20"
fi
while ! ping -c5 192.168.0.30
        do wget "https://smsapi.free-mobile.fr/sendmsg?user=16211308&pass=bLdaXQbP4f6DRn&msg=Alerte%20!%20Connexion%20perdue%20!!!"
done
exit
