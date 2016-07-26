
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "DHT.h"

#define lum 5
#define chauff 16

// Pin
#define DHTPINPC 2
#define DHTPINPF 4

// defini le capteur DHT22 
#define DHTTYPE DHT22

// Initialise les capteurs
DHT dhtPC(DHTPINPC, DHTTYPE);
DHT dhtPF(DHTPINPF, DHTTYPE);

// Host
const char* host = "192.168.0.2";

const char ssid[] = "freeman";  //  your network SSID (name)
const char pass[] = "manu2612@SOSSO1008";       // your network password

// NTP Servers:
static const char ntpServerName[] = "fr.pool.ntp.org";


const int timeZone = 1;     // Central European Time

long tempsterra;
long tempsenvoi;

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

void setup()
{
  Serial.begin(9600);  
  delay(100);
  Serial.println("Terrarium");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Init DHT 
  dhtPC.begin();
  dhtPF.begin();
  pinMode(lum, OUTPUT);
  pinMode(chauff, OUTPUT);
  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  tempsterra = millis();
  tempsenvoi =  millis();
  
}


void loop() {

  terrarium();     
  envoidata();
  
}

void terrarium() {

  if((millis() - tempsterra) > 10000) {

    int Hnow;
    int target;
    int Hmatin = 700;
    int Hsoir = 2100;
    
    Hnow = ((hour()+1) * 100) + minute();
    
      
    if ( Hnow > Hmatin && Hnow < Hsoir ) {  
    target = 28;
    digitalWrite(lum, LOW);   
    } else {    
      target = 23;     
      digitalWrite(lum, HIGH);           
       }                 
     
          // lire la sonde point chaud   
    float tC = dhtPC.readTemperature();
      // lire la sonde point froid 
    float tF = dhtPF.readTemperature();  
    
    if (target < tC or tF > 23 ) {        
      digitalWrite(chauff, HIGH);    
      } else {    
        digitalWrite(chauff, LOW);      
        }
        
    tempsterra = millis(); 
  }  
   
}

void envoidata() {

  if((millis() - tempsenvoi) > 60000) {

    Serial.print("Connecting to ");
    Serial.println(host);
    
    // creation de la connexion tcp
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
    
    // envoi les data par l'url au fichier php sur le serveur
    client.print(String("GET /dht22.php?tempC=") + String(float (tC)) + "&humiC=" + String(float(hC)) + "&tempF=" + String(float(tF)) + "&humiF=" + String(float(hF)) + " HTTP/1.1\r\n" +
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

    tempsenvoi = millis();
    }  
    
}


void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

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
