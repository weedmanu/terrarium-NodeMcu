#!/bin/bash

						#####   alerte par SMS si tempC < 22 °C  #####

# remplacez XXXX et YYYY par votre login et mot de pass de la bdd
mysql -u"XXXX" -p"YYYY" -Ddht22 -e "SELECT ROUND(AVG(tempC),0) as moytempC FROM temphumi GROUP BY DAY(date) DESC, HOUR(date) DESC LIMIT 1;" >> test.txt
tempC=`sed -n '$p' test.txt`
rm test.txt
# fonctionne uniquement si vous êtes chez free mobile, activer l'option de votre ligne et renseigné votre user et mdp de l'option à la place des XXXX et YYYY .
if  [[ $tempC < 22 ]]; then	
	wget "https://smsapi.free-mobile.fr/sendmsg?user=XXXXXX&pass=YYYYYY&msg=Alerte%20!%20Limite%20atteinte%20:%20tempC%20=%20$tempC%20°C%20"
fi


						#####     alerte par SMS si pb de com   #####
						
						
address=192.168.XXX.XXX # adresse du nodemcu
internet=1             # par defaut flag internet est up
i=0    # compteur
while [ $i -le 3 ]  # pour faire que 3 boucles sur non reponse au ping (Toujours perdue)
do    
    ping -c 1 ${address}  
    
    if [[ $? -ne 0 ]]; then
        if [[ ${internet} -eq 1 ]]; then   # flag -- etait up puis down
            # Connexion perdue
            wget "https://smsapi.free-mobile.fr/sendmsg?user=XXXXXX&pass=YYYYYY&msg=Alerte%20!%20Connexion%20perdue%20!!!"
        else				
			# Toujours perdue   			
			let $[ i+=1 ] # incremente i de 1 a chaque boucle       
        fi
        internet=0
    else
        if [[ ${internet} -eq 0 ]]; then     # flag -- etait down puis up
            # Connexion retrouvée
            wget "https://smsapi.free-mobile.fr/sendmsg?user=XXXXXX&pass=YYYYYY&msg=Connexion%20retrouvée%20!!!"
        fi
        internet=1
        break   # si la com est retouvée, on sort du while
    fi  
            
    sleep 100 ; # sleep 60 seconds = 1 min
    
	if [[ ${i} -eq 3 ]]; then       # si a la fin des 3 boucles i=3 alors on confirme que la com est perdue
		wget "https://smsapi.free-mobile.fr/sendmsg?user=XXXXXX&pass=YYYYYY&msg=Alerte%20!%20Connexion%20bien%20perdue,%20bouge%20toi%20le%cul%20!!!"
	fi
done
exit
