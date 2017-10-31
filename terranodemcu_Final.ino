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
const char* host = "192.168.0.57";  // adresse du serveur web (NAS synology chez moi)

//pour la page web 
const char* hostU = "terra";
const char* update_path = "/update";
const char* update_username = "manu";  // on defini un login pour la fonction update
const char* update_password = "manu@.13";  // votre mot de passe

//le wifi
const char ssid[] = "freeman";  //  votre SSID
const char pass[] = "manu2612@SOSSO1008";  // votre password

             //////////////////////////////////////////////////////
             
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

int PinHumi=13; // Broche Numérique mesure de l'humidité
const int buzzerPin = 15;
const int ledPin1 = 3;
const int ledPin2 = 1;
int alarm = 0; 
int counter = 0;
int secheresse;  //0 ou 1 si seuil atteint

// on initialise le seveur web
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

// le serveur NTP que l'on va interroger:
static const char ntpServerName[] = "fr.pool.ntp.org";

const int timeZone = 2;     // paris - bruxelle

//On défini le bouton
const int  buttonPin = 12;    

int buttonState = 0;         // état du bouton est a 0
int lastButtonState = 0;     // l'ancien état est a 0

//On déclare les variables pour gérer la boucle de chaque fonction du loop avec millis()
long tempsterra;
long tempsenvoi;
long tempssec;

WiFiUDP Udp;
unsigned int localPort = 8888;  // Le port d'Écoute des paquets UDP

// On déclare les variables
int Hnow;
int target;
int Hmatin;
int Hsoir;

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
  // on défini la page web pour l'upgrade
  MDNS.begin(hostU);
  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer prêt !!! Ouvre http://");
  Serial.print(WiFi.localIP());
  Serial.printf("/update dans ton navigateur\n", hostU);
  delay(1500);
    // on initialise les DHT 
  dhtPC.begin();
  dhtPF.begin();
   // on déclare les pins des relais en sortie
  pinMode(lum, OUTPUT);
  pinMode(chauff, OUTPUT);
  // le detecteur de secheresse en input, les leds et le buzzer en output
  pinMode(PinHumi, INPUT);  
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
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
  // on affiche sur le lcd la fin du setup
  lcd.backlight();  
  lcd.setCursor(3,0);
  lcd.print("Fin de l' ");
  lcd.setCursor(1,1);
  lcd.print("initialisation");
  delay(3000); 
  lcd.clear(); 
  lcd.noBacklight();  
  // on  donne le temps actuel aux variable pour le démarrage du timer des fonctions
  tempsterra = millis();
  tempsenvoi =  millis();
  tempssec = millis();
}

//******* la foction qui verifie si y a de l'eau toutes les 2 s 
void sec()
{
  if((millis() - tempssec) > 2000) {
    secheresse = digitalRead(PinHumi);
    if (secheresse==1)
    {
      alarm = alarm + 1;        // pour éviter de lancer l'alarme sur une fausse détéction 
    } else {
      alarm = 0;
    }
    if (alarm >= 3)              // si il y a 3 détections on lance l'alarme 
    {
      song();
    }     
     tempssec = millis();
  }  
}

//******   on lance les fonctions en boucle ******

 void loop() {
  httpServer.handleClient();
  terrarium();       
  envoidata();
  sec();
  bouton();
      
}

//******  la fonction qui s'occupe du chauffage et de la lumière ******
void terrarium() {

  if((millis() - tempsterra) > 5000) {  // si le temps actuel par rapport au temps de démarrage du timer est > 10 s ( toutes les 10 s )
    switch (month()) {           // suivant le mois, on modifie les heures :
      case 1: 
        Hmatin = 800;
        Hsoir = 1730;
      break;
      case 2: 
        Hmatin = 730;
        Hsoir = 1800;
      break;
      case 3: 
        Hmatin = 730;
        Hsoir = 1900;
      break;
      case 4: 
        Hmatin = 700;
        Hsoir = 2000;
      break;
      case 5: 
        Hmatin = 630;
        Hsoir = 2100;
      break;
      case 6: 
        Hmatin = 600;
        Hsoir = 2200;
      break;
      case 7: 
        Hmatin = 600;
        Hsoir = 2200;
      break;
      case 8: 
        Hmatin = 630;
        Hsoir = 2100;
      break;      
      case 9: 
        Hmatin = 700;
        Hsoir = 2030;
      break;
      case 10: 
        Hmatin = 730;
        Hsoir = 1930;
      break;
      case 11: 
        Hmatin = 730;
        Hsoir = 1800;
      break;
      case 12: 
        Hmatin = 800;
        Hsoir = 1730;
      break;             
    }
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
    
    if (target < tC) {        // si la température au point chaud dépasse la target (28 le jour ou 23 la nuit) et qu'au point froid il fait plus de 23 °C (pour l'été).
      digitalWrite(chauff, HIGH);         // on active le relais qui éteint le chauffage.
      } else {                            // sinon
        digitalWrite(chauff, LOW);        // on allume le chauffage.
        }
        
    tempsterra = millis();                // on réinitialise le timer 
  }  
   
}

//******  envoi des datas à la base de donnée ******

void envoidata() {

  if((millis() - tempsenvoi) > 18000) {   // si le temps actuel par rapport au temps de démarrage du timer est > 15 min ( toutes les 15 min )

    Serial.print("Connecting to ");         // on discute un peu sur le port série
    Serial.println(host);
    Serial.println("pour envoidata");
    
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
    
    // envoi les datas par l'URL par GET au fichier dht22.php sur le serveur (dans le dossier terranodemcu)
    client.print(String("GET /terrarium/terranodemcu/dht22.php?tempC=") + String(float (tC)) + "&humiC=" + String(float(hC)) + "&tempF=" + String(float(tF)) + "&humiF=" + String(float(hF)) + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
        
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
   delay(200);  
       
   lcd.setCursor(0,1);
   lcd.write(TAIL);
   lcd.setCursor(1,1);
   lcd.write(HEAD);
   delay(200);
       
   lcd.setCursor(1,1);
   lcd.write(TAIL);
   lcd.setCursor(2,1);
   lcd.write(HEAD);
   delay(200);
     
   lcd.setCursor(2,1);
   lcd.write(TAIL);
   lcd.setCursor(3,1);
   lcd.write(HEAD);
   delay(200);
     
   lcd.setCursor(3,1);
   lcd.write(TAIL);
   lcd.setCursor(4,1);
   lcd.write(HEAD);
   delay(200);
      
   lcd.setCursor(4,1);
   lcd.write(TAIL);
   lcd.setCursor(5,1);
   lcd.write(HEAD);
   delay(200);
       
   lcd.setCursor(5,1);
   lcd.write(TAIL);
   lcd.setCursor(6,1);
   lcd.write(HEAD);
   delay(200);
       
   lcd.setCursor(0,1);
   lcd.write(CLEAR);
   lcd.setCursor(6,1);
   lcd.write(TAIL);
   lcd.setCursor(7,0);
   lcd.write(HEAD);
   delay(200);
      
   lcd.setCursor(0,0);
   lcd.print("Terra");
   lcd.setCursor(1,1);
   lcd.write(CLEAR);
   lcd.setCursor(7,0);
   lcd.write(TAIL);
   lcd.setCursor(8,0);
   lcd.write(HEAD);
   delay(200);
   
   lcd.setCursor(2,1);
   lcd.write(CLEAR);
   lcd.setCursor(8,0);
   lcd.write(TAIL);
   lcd.setCursor(9,0);
   lcd.write(HEAD);
   delay(200);
   
   lcd.setCursor(3,1);
   lcd.write(CLEAR);
   lcd.setCursor(9,0);
   lcd.write(TAIL);
   lcd.setCursor(10,0);
   lcd.write(HEAD);
   delay(200);
   
   lcd.setCursor(4,1);
   lcd.write(CLEAR);
   lcd.setCursor(10,0);
   lcd.write(TAIL);
   lcd.setCursor(11,0);
   lcd.write(HEAD);
   delay(200);
   
   lcd.setCursor(5,1);
   lcd.write(CLEAR);
   lcd.setCursor(11,0);
   lcd.write(TAIL);
   lcd.setCursor(12,0);
   lcd.write(HEAD);
   delay(200);
   
   lcd.setCursor(6,1);
   lcd.write(CLEAR);
   lcd.setCursor(12,0);
   lcd.write(TAIL);
   lcd.setCursor(13,0);
   lcd.write(HEAD);
   delay(200);

   lcd.setCursor(7,1);
   lcd.print("NodeMcu");
   lcd.setCursor(7,0);
   lcd.write(CLEAR);
   lcd.setCursor(13,0);
   lcd.write(TAIL);
   lcd.setCursor(14,0);
   lcd.write(HEAD);
   delay(200);
   
   lcd.setCursor(8,0);
   lcd.write(CLEAR);
   lcd.setCursor(14,0);
   lcd.write(TAIL);
   lcd.setCursor(15,0);
   lcd.write(HEAD);
   delay(200); 
   
   lcd.setCursor(8,0);
   lcd.write(CLEAR);
   lcd.setCursor(15,0);
   lcd.write(TAIL);
   delay(200);
   
   lcd.setCursor(9,0);
   lcd.write(CLEAR);
   delay(200);
   
   lcd.setCursor(10,0);
   lcd.write(CLEAR);
   delay(200);
   
   lcd.setCursor(11,0);
   lcd.write(CLEAR);
   delay(200);
   
   lcd.setCursor(12,0);
   lcd.write(CLEAR);
   delay(200);
   
   lcd.setCursor(13,0);
   lcd.write(CLEAR);
   delay(200);
   
   lcd.setCursor(14,0);
   lcd.write(CLEAR);
   delay(200);
   
   lcd.setCursor(15,0);
   lcd.write(CLEAR); 
   lcd.clear(); 
   delay(500);

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
    delay(100);
    // on lance l'animation d'introduction
    intro();
    // puis on affiche les datas
    lcd.setCursor(3,0);
    lcd.print("Point chaud :");
    lcd.setCursor(0,1);
    lcd.print("Temp = ");
    lcd.print(float (tC));
    lcd.print(" C");
    delay(1500);    
    lcd.setCursor(0,1);
    lcd.print("Humi = ");
    lcd.print(float (hC));
    lcd.print(" %");
    delay(1500);
    lcd.clear();
    
    lcd.setCursor(3,0);
    lcd.print("Point froid :");
    lcd.setCursor(0,1);
    lcd.print("Temp = ");
    lcd.print(float (tF));
    lcd.print(" C");
    delay(1500);        
    lcd.setCursor(0,1);
    lcd.print("Humi = ");
    lcd.print(float (hF));
    lcd.print(" %");
    delay(1500);
    lcd.clear(); 
    delay(500);

    outro(); 
        
}

//****** sortie de l'affichage  ******

void outro () {
      
    delay(500);  
    lcd.setCursor(2,0);
    lcd.print("Bye bye !!!");
    lcd.setCursor(1,1);
    lcd.print("Terra-NodeMcu");
    delay(1500); 
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

//******* la partie song ****************

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note, duration);
 
  //Play different LED depending on value of 'counter'
  if(counter % 2 == 0)
  {
    digitalWrite(ledPin1, HIGH);
    delay(duration);
    digitalWrite(ledPin1, LOW);
  }else
  {
    digitalWrite(ledPin2, HIGH);
    delay(duration);
    digitalWrite(ledPin2, LOW);
  }
 
  //Stop tone on buzzerPin
  noTone(buzzerPin);
 
  delay(50);
 
  //Increment counter
  counter++;
}
 
void firstSection()
{
  beep(a, 500);
  beep(a, 500);    
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);  
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
 
  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);  
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
}
 
void secondSection()
{
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125);    
  beep(fSH, 250);
 
  delay(325);
 
  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325);  
  beep(cSH, 175);  
  beep(cH, 125);  
  beep(b, 125);  
  beep(cH, 250);  
 
  delay(350);
}

void song() 
{  
  //Play first section
  firstSection();
 
  //Play second section
  secondSection();
 
  //Variant 1
  beep(f, 250);  
  beep(gS, 500);  
  beep(f, 350);  
  beep(a, 125);
  beep(cH, 500);
  beep(a, 375);  
  beep(cH, 125);
  beep(eH, 650);
 
  delay(500);
 
  //Repeat second section
  secondSection();
 
  //Variant 2
  beep(f, 250);  
  beep(gS, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 650);  
 
  delay(650);
}

