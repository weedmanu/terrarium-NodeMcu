// les librairies utilisées 
#include <TimeLib.h>                   // pour le serveur NTP
#include <ESP8266WiFi.h>               // pour le réseau
#include <WiFiUdp.h>                   // pour interroger le serveur NTP
#include "DHT.h"                       // pour lire les sondes  
#include <Wire.h>                      // pour l'I2C 
#include <LiquidCrystal_I2C.h>         // pour l'écran LCD
#include <WiFiClient.h>                // pour se connecter à un serveur
#include <ESP8266WebServer.h>          // crée un serveur
#include <ESP8266mDNS.h>               // pour les DNS
#include <ESP8266HTTPUpdateServer.h>   // pour flasher le nodemcu par le web 

LiquidCrystal_I2C lcd(0x3F, 16, 2);    // paramétrage de l'écran : adresse I2C de l’écran (pour la trouver utiliser scanI2C.ino (voir tuto)) et 16 colonnes, 2 lignes

// défini le capteur DHT22 
#define DHTTYPE DHT22

// les notes de musique
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

// Pins des sondes
const int DHTPINPC = 14;
const int DHTPINPF = 0;

// Initialise les capteurs
DHT dhtPC(DHTPINPC, DHTTYPE);
DHT dhtPF(DHTPINPF, DHTTYPE);

// pins des relais
const int lum = 5;               
const int chauff = 16;

// détecteur de sécheresse
int PinHumi=13;  // sur broche digital
int secheresse;  //0 ou 1 si seuil atteint

// le buzzer
const int buzzerPin = 15;
const int ledPin1 = 3;
const int ledPin2 = 1;
int alarm = 0; 
int counter = 0;

              ///////partie à modifier avec vos paramètres \\\\\\\
// le Host 
const char* host = "192.168.XXX.XXX";         // adresse du serveur web (NAS synology chez moi)

//pour la page web 
const char* hostU = "TerraNodeMCU";
const char* update_path = "/update";
const char* update_username = "XXXX";      // on défini un login pour la fonction update
const char* update_password = "YYYY";      // votre mot de passe

//le wifi
const char ssid[] = "XXXXX";             //  votre SSID
const char pass[] = "YYYYY";             // votre password

             /////////////////////////    \\\\\\\\\\\\\\\\\\\\\\\\\\

// on initialise le serveur web
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

// le serveur NTP que l'on va interroger:
static const char ntpServerName[] = "fr.pool.ntp.org";
// la zone
int timeZone = 1;     // Paris - Bruxelles

//On défini le bouton
const int  buttonPin = 12;    

int buttonState = 0;         // état du bouton est a 0
int lastButtonState = 0;     // l'ancien état est a 0

//On déclare les variables pour gérer la boucle de chaque fonction du loop avec millis()
long tempsterra;
long tempsenvoi;
long tempssec;

// l'udp
WiFiUDP Udp;
unsigned int localPort = 8888;  // Le port d'Écoute des paquets UDP

// On déclare les variables
int Hnow;
int target;
int Hmatin;
int Hsoir;

// les tableaux pour gérer le décalage de l'éclairage en fonction des mois)
int mois[12] =  {1,2,3,4,5,6,7,8,9,10,11,12};
int matin[12] = {800,745,730,700,630,600,600,630,700,730,745,800};
int soir[12] = {1730,1800,1900,1930,2030,2130,2130,2030,1930,1900,1800,1730};

// on définie les variables où seront stockés les caractères perso TAIL HEAD CLEAR 
const int TAIL = 4;            
const int HEAD = 1;              
const int CLEAR = 2;   
  
// le dessin des caractères HEAD TAIL CLEAR
uint8_t snakeHead[8] = {0x11, 0xa, 0x1f, 0x15, 0xa, 0x11, 0xe};
uint8_t snakeTail[8] = {0x4, 0xe, 0x1b, 0x1b, 0xe, 0xe, 0x4};
uint8_t noSnake[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

void setup()
{
  // On démarre la com série 
  Serial.begin(115200);  
  delay(100);  
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
  delay(100);
  // on affiche l'IP attribué
  Serial.print("L'adresse IP assignée par DHCP est : ");
  Serial.println(WiFi.localIP());
  delay(100);
  // on défini la page web pour l'upgrade
  MDNS.begin(hostU);
  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
  delay(100);
  Serial.printf("Serveur prêt !!! Ouvre http://");  
  Serial.printf("%s.local/update dans ton navigateur\n", hostU);
  delay(1500);  
  // on discute encore un peu 
  Serial.println("Lancement de l'UDP");
  // on démarre l'écoute UDP
  Udp.begin(localPort);
  delay(100);
  Serial.print("Port local: ");
  Serial.println(Udp.localPort());
  Serial.println("attente de synchronisation");
  delay(100);
  // on récupère l'heure
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  delay(100);
    // on initialise les DHT 
  dhtPC.begin();
  dhtPF.begin();
   // on déclare les pins des relais en sortie
  pinMode(lum, OUTPUT);
  pinMode(chauff, OUTPUT);
  // les leds et le buzzer en sortie    
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  // le detecteur de secheresse en entrée
  pinMode(PinHumi, INPUT);   
  delay(100);  
  // initialise le GPIO du bouton en entrée avec la résistance de PULLUP: 
  pinMode(buttonPin, INPUT_PULLUP);
  // initialise les GPIO pour l'I2C
   Wire.begin(2,4);
  // initialise le LCD
  lcd.begin();  
  // éteint le retro-éclairage et efface l'écran
  lcd.noBacklight();
  lcd.clear();
   // création des caractères
  lcd.createChar(HEAD, snakeHead);
  lcd.createChar(TAIL, snakeTail);
  lcd.createChar(CLEAR, noSnake);
  delay(100);
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

//******   on lance les fonctions en boucle ******
 void loop() {
  changementHeure();
  httpServer.handleClient();
  terrarium();       
  envoidata();
  sec();
  bouton();      
}

//******* la fonction qui effectue le changement d'heure été/hiver
void changementHeure()
{
  if ( month() == 4 && day() > 24 && day() < 31 && weekday() == 7 && hour() == 3 && minute() == 0 && second() == 0)
    {    
       timeZone = 2; 
       setSyncProvider(getNtpTime);        
    }
  else if ( month() == 10 && day() > 24 && day() < 31 && weekday() == 7 && hour() == 3 && minute() == 0 && second() == 0)
    {    
       timeZone = 1; 
       setSyncProvider(getNtpTime);        
    }
}

//******* la fonction qui vérifie si y a de l'eau toutes les 2 s 
void sec()
{
  if((millis() - tempssec) > 2000) {
    secheresse = digitalRead(PinHumi);
    if (secheresse==1)
    {
      alarm = alarm + 1;        // pour éviter de lancer l'alarme sur une fausse détection 
    } else {
      alarm = 0;
    }
    if (alarm >= 3)              // si il y a 3 détections on lance l'alarme 
    {
      starwars();
    }     
     tempssec = millis();
  }  
}

//******  la fonction qui s'occupe du chauffage et de la lumière ******
void terrarium() {
  // si le temps actuel par rapport au temps de démarrage du timer est > 7s ( toutes les 7s )
  if((millis() - tempsterra) > 7000) {  // utilisation de nombre premier pour ne jamais lire les sondes en même temps que la fonction envoidata
    // suivant le mois, on modifie les heures :
    for (int i = 0; i<12; i++)
    { 
      if (month() == mois[i])
      {
        Hsoir = soir[i];
        Hmatin = matin[i];          
        Serial.println(mois[i]);
      }
    }
    // pour faciliter les calcul (21h03 devient 2103) 
    Hnow = hour() * 100 + minute(); 
      
    if ( Hnow > Hmatin && Hnow < Hsoir ) // si c'est le jour
    {  
      target = 28;                        // la consigne du chauffage est de 28 °C   
      digitalWrite(lum, LOW);             // on coupe le relais et le courant passe donc la lumière est allumée
    } 
    else                                  // sinon , donc c'est la nuit
    {                                
      target = 23;                        // la consigne du chauffage est de 23 °C
      digitalWrite(lum, HIGH);            // on active le relais, il coupe le courant, la lumière est éteinte
    }                 
     
    // on lis la sondes point chaud (température uniquement c'est pour la régulation du chauffage)
    float tC = dhtPC.readTemperature();    
    
    if (target < tC)                // si la température au point chaud dépasse la target (28 le jour ou 23 la nuit).
    {        
      digitalWrite(chauff, HIGH);   // on active le relais qui éteint le chauffage.
    } 
    else                            // sinon
    {                            
      digitalWrite(chauff, LOW);    // on allume le chauffage.
    }        
    tempsterra = millis();          // on réinitialise le timer 
  }     
}

//******  envoi des datas à la base de donnée ******
void envoidata() {
  // si le temps actuel par rapport au temps de démarrage du timer est > 17s ( toutes les 17s )
  if((millis() - tempsenvoi) > 17000) {   // utilisation de nombre premier pour ne jamais lire les sondes en même temps que la fonction terrarium
    Serial.print("Connexion à ");         // on discute un peu sur le port série
    Serial.println(host);
    Serial.println("pour envoidata");    
    // création de la connexion TCP
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("échec de la connexion");
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
    Serial.println("fermeture de la connexion");    
    tempsenvoi = millis();  // on réinitialise le timer 
    }     
}

//****** le bouton ******
void bouton() {
  // lit l'état du bouton 
  buttonState = digitalRead(buttonPin);

  // compare avec l'état précédant
  if (buttonState != lastButtonState) {
    // si l'état du bouton est a 1 (bouton relâché) on écrit off dans la com série et rien d'autre
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

//****** intro animation LCD ******

void intro() {
       
   lcd.setCursor(0,1);
   lcd.write(HEAD);   
   delay(200);  

   for (int i=0; i<6; i++)
   {
     int j = i+1;
     lcd.setCursor(i,1);
     lcd.write(TAIL);
     lcd.setCursor(j,1);
     lcd.write(HEAD);
     delay(200);
   }   
       
   lcd.setCursor(0,1);
   lcd.write(CLEAR);
   lcd.setCursor(6,1);
   lcd.write(TAIL);
   lcd.setCursor(7,0);
   lcd.write(HEAD);
   delay(200);
      
   lcd.setCursor(0,0);
   lcd.print("Terra");

   for (int i=1; i<7; i++)
   {
     int j = i+6;
     int k = j+1;
     lcd.setCursor(i,1);
     lcd.write(CLEAR);
     lcd.setCursor(j,0);
     lcd.write(TAIL);
     lcd.setCursor(k,0);
     lcd.write(HEAD);
     delay(200);
   }
   
   lcd.setCursor(7,1);
   lcd.print("NodeMcu");

   for (int i=7; i<9; i++)
   {
     int j = i+6;
     int k = j+1;
     lcd.setCursor(i,0);
     lcd.write(CLEAR);
     lcd.setCursor(j,0);
     lcd.write(TAIL);
     lcd.setCursor(k,0);
     lcd.write(HEAD);
     delay(200);
   }   
   
   lcd.setCursor(8,0);
   lcd.write(CLEAR);
   lcd.setCursor(15,0);
   lcd.write(TAIL);
   delay(200);

   for (int i=9; i<16; i++)
   {
     lcd.setCursor(i,0);
     lcd.write(CLEAR);
     delay(200);
   }
   
  lcd.clear();
}

//****** sortie animation LCD  ******
void outro () {          
    lcd.setCursor(2,0);
    lcd.print("Bye bye !!!");
    lcd.setCursor(1,1);
    lcd.print("Terra-NodeMcu");
    delay(1500); 
    lcd.clear(); 
    lcd.noBacklight();
 }

//****** affichage des datas des sondes en temps réel sur l'écran LCD ********
void affichage() {
      // lire la sonde point chaud
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
    // on lance l'animation 
    lcd.setCursor(3,0);
    lcd.print("Point chaud :");
    lcd.setCursor(0,1);
    lcd.print("Temp = ");
    lcd.print(float (tC));
    lcd.print(" ");
    lcd.write(223);
    lcd.print("C");
    delay(1500);    
    lcd.setCursor(0,1);
    lcd.print("Humi =  ");
    lcd.print(float (hC));
    lcd.print(" %");
    delay(1500);
    lcd.clear();
    
    lcd.setCursor(3,0);
    lcd.print("Point froid :");
    lcd.setCursor(0,1);
    lcd.print("Temp = ");
    lcd.print(float (tF));
    lcd.print(" ");
    lcd.write(223);
    lcd.print("C");
    delay(1500);        
    lcd.setCursor(0,1);
    lcd.print("Humi =  ");
    lcd.print(float (hF));
    lcd.print(" %");
    delay(1500);
    lcd.clear(); 
    delay(1000);
    // on lance l'animation de sortie
    outro();         
}


//****** la partie suivante n'est pas de moi, c'est l'exemple TimeNTP_ESP8266WIFI de la librairie Time. ( pour avoir l'heure par le réseau) ******

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

//****** la partie suivante n'est pas de moi, je l'ai trouvé ici : https://gist.github.com/nicksort/4736535

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

void starwars() 
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
