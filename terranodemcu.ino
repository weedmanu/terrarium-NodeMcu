// les librairies utilisées 

#include <TimeLib.h>                   // pour serveur NTP
#include <ESP8266WiFi.h>               // pour le reseau
#include <WiFiUdp.h>                   // pour interroger le serveur NTP
#include "DHT.h"                       // pour lire les sondes  

// pin des relais
#define lum 5               
#define chauff 16

// Pin des sondes

#define DHTPINPC 2
#define DHTPINPF 4

// defini le capteur DHT22 
#define DHTTYPE DHT22

// Initialise les capteurs
DHT dhtPC(DHTPINPC, DHTTYPE);
DHT dhtPF(DHTPINPF, DHTTYPE);

// le Host 
const char* host = "192.168.0.2";  // adresse du serveur web 

//le wifi
const char ssid[] = "votre SSID";  //  your network SSID (name)
const char pass[] = "votre Mot de passe";       // your network password

// le serveur NTP que l'on va interroger:
static const char ntpServerName[] = "fr.pool.ntp.org";


const int timeZone = 2;     // paris - bruxelle

// variable pour gérer la boucle de chaque fonction du loop avec millis()
long tempsterra;
long tempsenvoi;
long tempscsv;

WiFiUDP Udp;
unsigned int localPort = 8888;  // port d'Écoute des paquets UDP

time_t getNtpTime();               // heure NTP
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

void setup()   // le setup
{
  // demarre la com serie 
  Serial.begin(9600);  
  delay(100);
  // on discute un peu sur le port serie
  Serial.println("Terrarium");  
  Serial.print("Connexion à ");
  Serial.println(ssid);
  // on se conecte au reseau
  WiFi.begin(ssid, pass);  
  // lorsque l'on est connecte:
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  
  // on initialise les DHT 
  dhtPC.begin();
  dhtPF.begin();
   // on declrae les pins des relais en sortie
  pinMode(lum, OUTPUT);
  pinMode(chauff, OUTPUT);
  // on discute encore un peu
  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  // on demarre l'ecoute udp
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  // on recup l'heure
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  // on  donne le temps actuel aux variable pour le demmarage du timer
  tempsterra = millis();
  tempsenvoi =  millis();
  tempscsv = millis();
  
}


void loop() {
  // on lance les fonctions en boucle
  terrarium();       
  envoibdd();
  datacsv();
  
}

/*-------- terrarium ----------*/

void terrarium() {

  if((millis() - tempsterra) > 10000) {  // si le temps actuel par rapport au temps de demarage du timer est > 10 s

    int Hnow;             // on declare les variables
    int target;
    int Hmatin = 700;
    int Hsoir = 2100;
    
    Hnow = hour() * 100 + minute(); // pour faciliter les calcul (21h03 devient 2103) 
      
    if ( Hnow > Hmatin && Hnow < Hsoir ) {  // si c'est le jour
    target = 28;                            // la consigne du chauffage est de 28 °C   
    digitalWrite(lum, LOW);                 // on active pas le relais et le courant passe donc la lumiere est allumée
    } else {                                // sinon , donc c'est la nuit
      target = 23;                          // la consigne du chauffage est de 23 °C
      digitalWrite(lum, HIGH);              // on active le relais, il coupe le courant, la lumiere est eteinte
       }                 
     
          // lire les sondes (temperature uniquement)
    float tC = dhtPC.readTemperature(); // (pointchaud)
      // lire la sonde point froid 
    float tF = dhtPF.readTemperature(); // (pointfroid) 
    
    if (target < tC or tF > 23 ) {        // si la temperature au point chaud dépasse la target (28 le jour ou 23 la nuit) et qu'au point froid il fait plus de 23 °C (pour l'été).
      digitalWrite(chauff, HIGH);         // on active le relais qui éteint le chauffage.
      } else {                            // sinon
        digitalWrite(chauff, LOW);        // on allume le chauffage.
        }
        
    tempsterra = millis();                // on réinitialise le timer 
  }  
   
}

/*-------- envoibdd ----------*/

void envoibdd() {

  if((millis() - tempsenvoi) > 60000*15) {   // si le temps actuel par rapport au temps de demarage du timer est > 15 min

    Serial.print("Connecting to ");         // on discute un peu sur le port serie
    Serial.println(host);
    Serial.println("pour envoibdd");
    
    // creation de la connexion tcp
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }
    
    // lire les sondes 
    float hC = dhtPC.readHumidity();  
    float tC = dhtPC.readTemperature();
      // lire la sonde point froid
    float hF = dhtPF.readHumidity();  
    float tF = dhtPF.readTemperature();
    
    // envoi les data par l'url au fichier dht22bdd.php sur le serveur
    client.print(String("GET /terranodemcu/dht22bdd.php?tempC=") + String(float (tC)) + "&humiC=" + String(float(hC)) + "&tempF=" + String(float(tF)) + "&humiF=" + String(float(hF)) + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
    delay(10);
    
    // Lire toutes les lignes de la réponse du serveur et fermer la connexion
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
      
    Serial.println();
    Serial.println("closing connection");    

    tempsenvoi = millis();  // on réinitialise le timer 
    }  
    
}

/*-------- datacsv ----------*/

void datacsv() {

  if((millis() - tempscsv) > 30000) {         // si le temps actuel par rapport au temps de demarage du timer est > 30 secondes

    Serial.print("Connecting to ");          // on discute un peu sur le port serie
    Serial.println(host);
    Serial.println("pour envoicsv");
    
    // creation de la connexion tcp    
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }
    
    // lire les sondes
    float hC = dhtPC.readHumidity();  
    float tC = dhtPC.readTemperature();
      // lire la sonde point froid
    float hF = dhtPF.readHumidity();  
    float tF = dhtPF.readTemperature();
    
    // envoi les data par l'url au fichier dht22csv.php sur le serveur
    client.print(String("GET /terranodemcu/dht22csv.php?tempC=") + String(float (tC)) + "&humiC=" + String(float(hC)) + "&tempF=" + String(float(tF)) + "&humiF=" + String(float(hF)) + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
    delay(10);
    
    // Lire toutes les lignes de la réponse du serveur et fermer la connexion
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
      
    Serial.println();
    Serial.println("closing connection"); 

    tempscsv = millis();       // on réinitialise le timer 
    }  
    
}

// la partie suivante n'est pas de moi , c'est l'exemple TimeNTP_ESP8266WIFI de la librairie Time.

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
