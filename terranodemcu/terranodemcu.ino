// les librairies utilisées 

#include <TimeLib.h>                   // pour le serveur NTP
#include <ESP8266WiFi.h>               // pour le réseau
#include <WiFiUdp.h>                   // pour interroger le serveur NTP
#include "DHT.h"                       // pour lire les sondes  
#include <Wire.h>                      // pour l'I2C 
#include <LiquidCrystal_I2C.h>         // pour l'écran lcd
#include <WiFiClient.h>                // pour se connecter à un serveur
#include <ESP8266WebServer.h>          // crée un serveur
#include <ESP8266mDNS.h>               // pour les DNS
#include <ESP8266HTTPUpdateServer.h>   // pour flasher le nodemcu par le web 

LiquidCrystal_I2C lcd(0x3F, 16, 2);    // paramétrage de l'écran : adresse I2C de lécran (pour la trouver utiliser scanI2C.ino (voir tuto)) et 16 colonnes, 2 lignes

// pin des relais
#define lum 5               
#define chauff 16

// Pin des sondes

#define DHTPINPC 14
#define DHTPINPF 0

// défini le capteur DHT22 
#define DHTTYPE DHT22

// Initialise les capteurs
DHT dhtPC(DHTPINPC, DHTTYPE);
DHT dhtPF(DHTPINPF, DHTTYPE);

              ///////partie à modifier avec vos paramètres \\\\\\\
// le Host 
const char* host = "192.168.0.2";  // adresse du serveur web (NAS synology chez moi)

//pour la page web 
const char* hostU = "webupdate"; 
const char* update_path = "/firmware";
const char* update_username = "manu";  // votre identifiant pour accéder à la page 
const char* update_password = "manu@.13";  // votre mote de passe 

//le wifi
const char ssid[] = "freeman";  //  votre SSID
const char pass[] = "manu2612@SOSSO1008";       // votre clé wi-fi

             //////////////////////////////////////////////////////

// on initialise le seveur web
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

// le serveur NTP que l'on va interroger:
static const char ntpServerName[] = "fr.pool.ntp.org";

const int timeZone = 1;     // paris - bruxelle

//On défini le bouton
const int  buttonPin = 12;    

int buttonState = 0;         // état du bouton est a 0
int lastButtonState = 0;     // l'ancien état est a 0

//On déclare les variables pour gérer la boucle de chaque fonction du loop avec millis()
long tempsterra;
long tempsenvoi;
long tempscsv;

WiFiUDP Udp;
unsigned int localPort = 8888;  // Le port d'Écoute des paquets UDP


void setup()
{
  // On démarre la com série 
  Serial.begin(115200);  
  delay(100);
  // initialise le GPIO du bouton en input PULLUP: 
  pinMode(buttonPin, INPUT_PULLUP);
  // initialise les GPIO pour l'I2C
   Wire.begin(2,4);
  // initialise le LCD
  lcd.begin();  
  // éteint le retro-éclairage et efface l'écran
  lcd.noBacklight();
  lcd.clear();
  // on discute un peu sur le port série
  Serial.println("Terrarium");  
  Serial.print("Connexion à ");
  Serial.println(ssid);
  // on se connecte au réseau
  WiFi.begin(ssid, pass);  
  // on attend la connexion :
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  // on affiche l'IP attribué
  Serial.print("L'adresse IP assignée par DHCP est : ");
  Serial.println(WiFi.localIP());
  // on défini la page web
  MDNS.begin(hostU);
  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer prêt !!! Ouvre http://");
  Serial.print(WiFi.localIP());
  Serial.printf("/update dans ton navigateur\n", hostU);
    // on initialise les DHT 
  dhtPC.begin();
  dhtPF.begin();
   // on déclare les pins des relais en sortie
  pinMode(lum, OUTPUT);
  pinMode(chauff, OUTPUT);
  // on discute encore un peu 
  Serial.println("Lancement de l'UDP");
  // on démarre l'écoute UDP
  Udp.begin(localPort);
  Serial.print("Port local: ");
  Serial.println(Udp.localPort());
  Serial.println("attente de synchronisation");
  // on récupère l'heure
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  // on  donne le temps actuel aux variable pour le démarrage du timer des fonctions
  tempsterra = millis();
  tempsenvoi =  millis();
  tempscsv = millis();
  
}


 //******   on lance les fonctions en boucle ******

 void loop() {
  httpServer.handleClient();
  terrarium();       
  envoibdd();
  datacsv();
  bouton();
}


//******  la fonction qui s'occupe du chauffage et de la lumière ******
void terrarium() {

  if((millis() - tempsterra) > 10000) {  // si le temps actuel par rapport au temps de démarrage du timer est > 10 s ( toutes les 10 s )

     // On déclare les variables
    int Hnow;
    int target;
    int Hmatin = 730;           // à modifier selon vos souhaits.
    int Hsoir = 1730;           // à modifier selon vos souhaits.
    
    Hnow = hour() * 100 + minute(); // pour faciliter les calcul (21h03 devient 2103) 
      
    if ( Hnow > Hmatin && Hnow < Hsoir ) {  // si c'est le jour
    target = 28;                            // la consigne du chauffage est de 28 °C   
    digitalWrite(lum, LOW);                 // on active pas le relais et le courant passe donc la lumière est allumée
    } else {                                // sinon , donc c'est la nuit
      target = 23;                          // la consigne du chauffage est de 23 °C
      digitalWrite(lum, HIGH);              // on active le relais, il coupe le courant, la lumière est éteinte
       }                 
     
          // lis les sondes (température uniquement)
    float tC = dhtPC.readTemperature(); // (point chaud)
      
    float tF = dhtPF.readTemperature(); // (point froid) 
    
    if (target < tC or tF > 23 ) {        // si la température au point chaud dépasse la target (28 le jour ou 23 la nuit) et qu'au point froid il fait plus de 23 °C (pour l'été).
      digitalWrite(chauff, HIGH);         // on active le relais qui éteint le chauffage.
      } else {                            // sinon
        digitalWrite(chauff, LOW);        // on allume le chauffage.
        }
        
    tempsterra = millis();                // on réinitialise le timer 
  }  
   
}

//******  envoi des datas à la base de donnée ******

void envoibdd() {

  if((millis() - tempsenvoi) > 60000*15) {   // si le temps actuel par rapport au temps de démarrage du timer est > 15 min ( toutes les 15 min )

    Serial.print("Connecting to ");         // on discute un peu sur le port série
    Serial.println(host);
    Serial.println("pour envoibdd");
    
    // création de la connexion TCP
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }
    
    // lire la sonde point chaud 
    float hC = dhtPC.readHumidity();  
    float tC = dhtPC.readTemperature();
      // lire la sonde point froid
    float hF = dhtPF.readHumidity();  
    float tF = dhtPF.readTemperature();
    
    // envoi les datas par l'URL par GET au fichier dht22bdd.php sur le serveur (dans le dossier terranodemcu)
    client.print(String("GET /terranodemcu/dht22bdd.php?tempC=") + String(float (tC)) + "&humiC=" + String(float(hC)) + "&tempF=" + String(float(tF)) + "&humiF=" + String(float(hF)) + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
    delay(10);
    
    // Lire toutes les lignes de la réponse du serveur, les écrire et fermer la connexion.
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
      
    Serial.println();
    Serial.println("closing connection");    

    tempsenvoi = millis();  // on réinitialise le timer 
    }  
    
}

//******  écriture des datas dans un csv sur le serveur pour les jauges (on incrément pas le fichier on écrase les valeurs) ******

void datacsv() {

  if((millis() - tempscsv) > 30000) {         // si le temps actuel par rapport au temps de démarrage du timer est > 30 secondes

    Serial.print("Connecting to ");          // on discute un peu sur le port série
    Serial.println(host);
    Serial.println("pour envoicsv");
    
    // création de la connexion TCP   
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }
    
    // lire la sonde point chaud
    float hC = dhtPC.readHumidity();  
    float tC = dhtPC.readTemperature();
      // lire la sonde point froid
    float hF = dhtPF.readHumidity();  
    float tF = dhtPF.readTemperature();
    
    // envoi les data par l'URL au fichier dht22csv.php sur le serveur (dans le dossier terranodemcu)
    client.print(String("GET /terranodemcu/dht22csv.php?tempC=") + String(float (tC)) + "&humiC=" + String(float(hC)) + "&tempF=" + String(float(tF)) + "&humiF=" + String(float(hF)) + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
    delay(10);
    
    // Lire et écrire sur le port série toutes les lignes de la réponse du serveur et fermer la connexion
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
      
    Serial.println();
    Serial.println("closing connection"); 

    tempscsv = millis();       // on réinitialise le timer 
    }  
    
}

//****** le bouton ******

void bouton() {
  // lit l'état du bouton 
  buttonState = digitalRead(buttonPin);

  // compare avec l'état précédant
  if (buttonState != lastButtonState) {
    // si l'etat du bouton est a 1 (bouton relâché) on ecrit off dans la com serie et rien d 'autre
    if (buttonState == HIGH) {
      Serial.println("off");
    } else {
      // si l'état du bouton est a 0 (bouton enclenché) on écrit on dans la com série et lance la fonction d'affichage      
      Serial.println("on");
      affichage();
      delay(1000);
    }
    // Delay pour la sensibilité du bouton 
    delay(50);
  }
  // sauve l'état du bouton pour la réinitialiser la boucle  
  lastButtonState = buttonState;

}

//****** animation LCD avant l'affichage des datas ******

void intro() {

// on définie les variables où seront stockés les caractères perso TAIL HEAD CLEAR 

  const int TAIL = 4;            
  const int HEAD = 1;              
  const int CLEAR = 2;              
 

//le dessin des caractères HEAD TAIL CLEAR

  byte snakeHead[8] = {     // head
     B10001,
     B01010,
     B11111,
     B10101,
     B11111,
     B10101,
     B10001,
     B01110
  };

  byte snakeTail[8] = {    // tail
     B00100,
     B01110,
     B01110,
     B11011,
     B11011,
     B01110,
     B01110,
     B00100
  };

byte noSnake[8] = {        // Clear 
     B00000,
     B00000,
     B00000,
     B00000,
     B00000,
     B00000,
     B00000,
     B00000
  };

// création des caractères

  lcd.createChar(HEAD, snakeHead);
  lcd.createChar(TAIL, snakeTail);
  lcd.createChar(CLEAR, noSnake);

// l'animation en elle même.

       
   lcd.setCursor(0,1);
   lcd.write(HEAD);   
   delay(250);  
       
   lcd.setCursor(0,1);
   lcd.write(TAIL);
   lcd.setCursor(1,1);
   lcd.write(HEAD);
   delay(250);
       
   lcd.setCursor(1,1);
   lcd.write(TAIL);
   lcd.setCursor(2,1);
   lcd.write(HEAD);
   delay(250);
     
   lcd.setCursor(2,1);
   lcd.write(TAIL);
   lcd.setCursor(3,1);
   lcd.write(HEAD);
   delay(250);
     
   lcd.setCursor(3,1);
   lcd.write(TAIL);
   lcd.setCursor(4,1);
   lcd.write(HEAD);
   delay(250);
      
   lcd.setCursor(4,1);
   lcd.write(TAIL);
   lcd.setCursor(5,1);
   lcd.write(HEAD);
   delay(250);
       
   lcd.setCursor(5,1);
   lcd.write(TAIL);
   lcd.setCursor(6,1);
   lcd.write(HEAD);
   delay(250);
       
   lcd.setCursor(0,1);
   lcd.write(CLEAR);
   lcd.setCursor(6,1);
   lcd.write(TAIL);
   lcd.setCursor(7,0);
   lcd.write(HEAD);
   delay(250);
      
   lcd.setCursor(0,0);
   lcd.print("Terra");
   lcd.setCursor(1,1);
   lcd.write(CLEAR);
   lcd.setCursor(7,0);
   lcd.write(TAIL);
   lcd.setCursor(8,0);
   lcd.write(HEAD);
   delay(250);
   
   lcd.setCursor(2,1);
   lcd.write(CLEAR);
   lcd.setCursor(8,0);
   lcd.write(TAIL);
   lcd.setCursor(9,0);
   lcd.write(HEAD);
   delay(250);
   
   lcd.setCursor(3,1);
   lcd.write(CLEAR);
   lcd.setCursor(9,0);
   lcd.write(TAIL);
   lcd.setCursor(10,0);
   lcd.write(HEAD);
   delay(250);
   
   lcd.setCursor(4,1);
   lcd.write(CLEAR);
   lcd.setCursor(10,0);
   lcd.write(TAIL);
   lcd.setCursor(11,0);
   lcd.write(HEAD);
   delay(250);
   
   lcd.setCursor(5,1);
   lcd.write(CLEAR);
   lcd.setCursor(11,0);
   lcd.write(TAIL);
   lcd.setCursor(12,0);
   lcd.write(HEAD);
   delay(250);
   
   lcd.setCursor(6,1);
   lcd.write(CLEAR);
   lcd.setCursor(12,0);
   lcd.write(TAIL);
   lcd.setCursor(13,0);
   lcd.write(HEAD);
   delay(250);

   lcd.setCursor(7,1);
   lcd.print("NodeMcu");
   lcd.setCursor(7,0);
   lcd.write(CLEAR);
   lcd.setCursor(13,0);
   lcd.write(TAIL);
   lcd.setCursor(14,0);
   lcd.write(HEAD);
   delay(250);
   
   lcd.setCursor(8,0);
   lcd.write(CLEAR);
   lcd.setCursor(14,0);
   lcd.write(TAIL);
   lcd.setCursor(15,0);
   lcd.write(HEAD);
   delay(250); 
   
   lcd.setCursor(8,0);
   lcd.write(CLEAR);
   lcd.setCursor(15,0);
   lcd.write(TAIL);
   delay(250);
   
   lcd.setCursor(9,0);
   lcd.write(CLEAR);
   delay(250);
   
   lcd.setCursor(10,0);
   lcd.write(CLEAR);
   delay(250);
   
   lcd.setCursor(11,0);
   lcd.write(CLEAR);
   delay(250);
   
   lcd.setCursor(12,0);
   lcd.write(CLEAR);
   delay(250);
   
   lcd.setCursor(13,0);
   lcd.write(CLEAR);
   delay(250);
   
   lcd.setCursor(14,0);
   lcd.write(CLEAR);
   delay(250);
   
   lcd.setCursor(15,0);
   lcd.write(CLEAR); 
   lcd.clear(); 
   delay(1000);

}
//****** affichage des datas des sondes en temps réel sur l'écran LCD ********

void affichage() {
      // lire la sondes point chaud
    float hC = dhtPC.readHumidity();  
    float tC = dhtPC.readTemperature();
      // lire la sonde point froid
    float hF = dhtPF.readHumidity();  
    float tF = dhtPF.readTemperature();

    // on allume le retro-éclairage
    lcd.backlight();
    delay(500);
    // on lance l'animation d'introduction
    intro();
    // puis on affiche les datas
    lcd.setCursor(3,0);
    lcd.print("Point chaud :");
    lcd.setCursor(0,1);
    lcd.print("Temp = ");
    lcd.print(float (tC));
    lcd.print(" C");
    delay(4000);    
    lcd.setCursor(0,1);
    lcd.print("Humi = ");
    lcd.print(float (hC));
    lcd.print(" %");
    delay(4000);
    lcd.clear();
    
    lcd.setCursor(3,0);
    lcd.print("Point froid :");
    lcd.setCursor(0,1);
    lcd.print("Temp = ");
    lcd.print(float (tF));
    lcd.print(" C");
    delay(4000);        
    lcd.setCursor(0,1);
    lcd.print("Humi = ");
    lcd.print(float (hF));
    lcd.print(" %");
    delay(4000);
    lcd.clear(); 
    delay(1000);

    outro(); 
        
}

//****** sortie de l'affichage  ******

void outro () {
      
    delay(1000);  
    lcd.setCursor(2,0);
    lcd.print("Bye bye !!!");
    lcd.setCursor(1,1);
    lcd.print("Terra-NodeMcu");
    delay(3000); 
    lcd.clear(); 
    lcd.noBacklight();

 }



//****** la partie suivante n'est pas de moi , c'est l'exemple TimeNTP_ESP8266WIFI de la librairie Time. ( pour avoir l'heure par le réseau) ******

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
