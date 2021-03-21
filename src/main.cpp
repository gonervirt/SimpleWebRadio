/**
 

  //	ESP32dev Signal     	ESP8266	   Wired to VS1053      
// 	-------- ------    	------------  	-------------------  
// 	GPIO32             	GPIO 16 D0	    pin 1 XDCS           
// 	GPIO5              	GPIO 5 D1	      pin 2 XCS            
// 	GPIO4              	GPIO 4 D2	      pin 4 DREQ           
// 	GPIO2              		-                    
// 	GPIO22             		-                    
// 	GPIO16   RXD2      		-                    
// 	GPIO17   TXD2      		-                    
// 	GPIO18   SCK       	D5 (GPIO 14)   	pin 5 SCK            
// 	GPIO19   MISO      	D6 (GPIO 12)  	pin 7 MISO           
// 	GPIO23   MOSI      	D7 (GPIO 13)	  pin 6 MOSI           
// 	GPIO15             		-                    
// 	GPI03    RXD0      		-                    
// 	GPIO1    TXD0      		-                    
// 	GPIO34   -         		-                    
// 	GPIO35   -         		-                    
// 	GPIO25   -         		-                    
// 	GPIO26   -         		-                    
// 	GPIO27   -         		-                    
// 	-------  ------    		-----------    --------  
// 	GND      -         		GND             pin 8 GND            
// 	VCC 5 V  -         		-                    
// 	VCC 5 V  -         		VIN             pin 9 5V             
// 	EN       -         	  RST             RST	pin 3 XRST           


 / ESP32dev Signal  	 Wired to VS1053     	Wired to the rest
// -------- ------ 	  -------------------	 ---------------
// GPIO32          	  pin 1 XDCS         	 -
// GPIO5           	  pin 2 XCS          	 -
// GPIO4           	  pin 4 DREQ         	 -
// GPIO2           	  -                  	 -
// GPIO22          	  -                  	 -
// GPIO16   RXD2   	  -                  	 TX of NEXTION (if in use)
// GPIO17   TXD2   	  -                  	 RX of NEXTION (if in use)
// GPIO18   SCK    	  pin 5 SCK          	 -
// GPIO19   MISO   	  pin 7 MISO         	 -
// GPIO23   MOSI   	  pin 6 MOSI         	 -
// GPIO15          	  -                  	 -
// GPI03    RXD0   	  -                  	 Reserved serial input
// GPIO1    TXD0   	  -                  	 Reserved serial output
// GPIO34   -      	  -                  	 Optional pull-up resistor
// GPIO35   -      	  -                  	 Infrared receiver VS1838B
// GPIO25   -      	  -                  	 Rotary encoder CLK
// GPIO26   -      	  -                  	 Rotary encoder DT
// GPIO27   -      	  -                  	 Rotary encoder SW
// -------  ------ 	  -------------------	 ----------------
// GND      -      	  pin 8 GND          	 Power supply GND
// VCC 5 V  -      	  -                  	 Power supply
// VCC 5 V  -      	  pin 9 5V           	 Power supply
// EN       -      	  pin 3 XRST         	 -
 
  
// ESP32dev Signal  Wired to LCD        Wired to VS1053      SDCARD   Wired to the rest
// -------- ------  --------------      -------------------  ------   ---------------
// GPIO32           -                   pin 1 XDCS            -       -
// GPIO5            -                   pin 2 XCS             -       -
// GPIO4            -                   pin 4 DREQ            -       -
// GPIO2            pin 3 D/C or A0     -                     -       -
// GPIO22           -                   -                     CS      -
// GPIO16   RXD2    -                   -                     -       TX of NEXTION (if in use)
// GPIO17   TXD2    -                   -                     -       RX of NEXTION (if in use)
// GPIO18   SCK     pin 5 CLK or SCK    pin 5 SCK             CLK     -
// GPIO19   MISO    -                   pin 7 MISO            MISO    -
// GPIO23   MOSI    pin 4 DIN or SDA    pin 6 MOSI            MOSI    -
// GPIO15           pin 2 CS            -                     -       -
// GPI03    RXD0    -                   -                     -       Reserved serial input
// GPIO1    TXD0    -                   -                     -       Reserved serial output
// GPIO34   -       -                   -                     -       Optional pull-up resistor
// GPIO35   -       -                   -                     -       Infrared receiver VS1838B
// GPIO25   -       -                   -                     -       Rotary encoder CLK
// GPIO26   -       -                   -                     -       Rotary encoder DT
// GPIO27   -       -                   -                     -       Rotary encoder SW
// -------  ------  ---------------     -------------------  ------   ----------------
// GND      -       pin 8 GND           pin 8 GND                     Power supply GND
// VCC 5 V  -       pin 7 BL            -                             Power supply
// VCC 5 V  -       pin 6 VCC           pin 9 5V                      Power supply
// EN       -       pin 1 RST           pin 3 XRST                    -
//
  --------------------------------
  | VS1053  | ESP8266 |  ESP32   |
  --------------------------------
  |   SCK   |   D5    |   IO18   |
  |   MISO  |   D6    |   IO19   |
  |   MOSI  |   D7    |   IO23   |
  |   XRST  |   RST   |   EN     |
  |   CS    |   D1    |   IO5    |
  |   DCS   |   D0    |   IO16   |
  |   DREQ  |   D3    |   IO4    |
  |   5V    |   5V    |   5V     |
  |   GND   |   GND   |   GND    |
  --------------------------------

  Dependencies:
  -VS1053 library by baldram (https://github.com/baldram/ESP_VS1053_Library)
  -ESP8266Wifi/WiFi

  To run this example define the platformio.ini as below.

test 
*/

#include <VS1053.h>
#include <WiFiManager.h>




//#include <WiFi.h>

#ifdef ESP32
#define VS1053_CS     5
#define VS1053_DCS    32
#define VS1053_DREQ   4
#define BUTTON_STATION1      27
#define BUTTON_STATION2      26
#define BUTTON_STATION3      25
#endif

#ifdef ESP8266
#define VS1053_CS     5
#define VS1053_DCS    16
#define VS1053_DREQ   4
#define BUTTON_STATION1      5
#define BUTTON_STATION2      18
#define BUTTON_STATION3      19
#endif


// Default volume
#define VOLUME  80

VS1053 player(VS1053_CS, VS1053_DCS, VS1053_DREQ);
WiFiClient client;


//  http://comet.shoutca.st:8563/1
//const char *host = "airspectrum.cdnstream1.com";
//const char *path = "/1261_192";
//int httpPort = 8000;
const char *station1="http://airspectrum.cdnstream1.com:8000/1261_192";
const char *station2="http://icecast.rtl2.fr:80/rtl2-1-44-128?listen=webCwsBCggNCQgLDQUGBAcGBg";
const char *station3="http://hitwest.ice.infomaniak.ch:80/hitwest-high.mp3";


const char*currentStation = station1;
int currentButton=BUTTON_STATION1;

// The buffer size 64 seems to be optimal. At 32 and 128 the sound might be brassy.
uint8_t mp3buff[64];

void connectWebRadio(const char *url)
{
  
  /*printf("ip = \"%s\"\n", ip);
  printf("port = \"%d\"\n", port);
  printf("page = \"\\%s\"\n", page);
  */

  if (!client.connected())
  {
    // split url
    char host[100];
    int httpPort = 80;
    char path[100];
    sscanf(url, "http://%99[^:]:%99d/%99[^\n]", host, &httpPort, path);
    Serial.print("connecting to ");
    Serial.print("http://");
    Serial.print(host);
    Serial.print(":");
    Serial.println(httpPort);

    if (!client.connect(host, httpPort))
    {
      Serial.println("Connection failed");
      return;
    }

    Serial.print("Requesting stream: ");
   // Serial.println("http://"+host+":"+httpPort+path);
    Serial.print("http://");
    Serial.print(host);
    Serial.print(":");
    Serial.print(httpPort);
    Serial.print("/");
    Serial.println(path);
  
    client.print(String("GET ") + "/" + path + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
  }
}

void checkButton ()
{
    if (digitalRead(BUTTON_STATION1) == HIGH) {
      // Button station 1 pressed
      if (currentButton != BUTTON_STATION1) {
        // on change de station
        currentButton = BUTTON_STATION1;
        currentStation = station1;
        // disconnect current web connection
        client.stop();
      }
    }
    if (digitalRead(BUTTON_STATION2) == HIGH) {
      // Button station 1 pressed
      if (currentButton != BUTTON_STATION2) {
        // on change de station
        currentButton = BUTTON_STATION2;
        currentStation = station2;
        // disconnect current web connection
        client.stop();
      }
    }
    if (digitalRead(BUTTON_STATION3) == HIGH) {
      // Button station 1 pressed
      if (currentButton != BUTTON_STATION3) {
        // on change de station
        currentButton = BUTTON_STATION3;
        currentStation = station3;
        // disconnect current web connection
        client.stop();
      }
    }
}
  

  void setup()
  {
    

    Serial.begin(115200);

    // Wait for VS1053 and PAM8403 to power up
    // otherwise the system might not start up correctly
    delay(3000);

    // This can be set in the IDE no need for ext library
    // system_update_cpu_freq(160);

    Serial.println("\n\nSimple Radio Node WiFi Radio");

    SPI.begin();

    player.begin();
    player.switchToMp3Mode();
    player.setVolume(VOLUME);

   WiFi.mode(WIFI_STA);
   //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;
    wm.autoConnect("AutoConnectAP",""); // password protected ap

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

  // init button pour changer station
    pinMode(BUTTON_STATION1, INPUT);
    pinMode(BUTTON_STATION2, INPUT);
    pinMode(BUTTON_STATION3, INPUT);

 
    //connectWebRadio("http://comet.shoutca.st:8563/1");

    /*
    Serial.print("connecting to ");
    Serial.println(host);

    if (!client.connect(host, httpPort)) {
        Serial.println("Connection failed");
        return;
    }

    Serial.print("Requesting stream: ");
    Serial.println(path);

    client.print(String("GET ") + path + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    */
}

void loop() {
    checkButton();

    connectWebRadio(currentStation);
    /*
    if (!client.connected()) {
        Serial.println("Reconnecting...");
        if (client.connect(host, httpPort)) {
            client.print(String("GET ") + path + " HTTP/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Connection: close\r\n\r\n");
        }
    }
    */

    if (client.available() > 0) {
        // The buffer size 64 seems to be optimal. At 32 and 128 the sound might be brassy.
        uint8_t bytesread = client.read(mp3buff, 64);
        player.playChunk(mp3buff, bytesread);
    }
}
