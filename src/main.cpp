/**
 

  //	ESP32dev Signal     	ESP8266	   Wired to VS1053      I2C screen
// 	-------- ------    	------------  	-------------------  
// 	GPIO32             	GPIO 16 D0	    pin 1 XDCS           
// 	GPIO5              	GPIO 5 D1	      pin 2 XCS            
// 	GPIO4              	GPIO 4 D2	      pin 4 DREQ           
// 	GPIO2              		-                    
//  GPIO21                                                  SDA
// 	GPIO22                                                  SCL
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
// 	GND      -         		GND             pin 8 GND         GND   
// 	VCC 5 V  -         		-                    
// 	VCC 5 V  -         		VIN             pin 9 5V          VCC   
// 	EN       -         	  RST             RST	pin 3 XRST           


  Dependencies:
  -VS1053 library by baldram (https://github.com/baldram/ESP_VS1053_Library)
  -ESP8266Wifi/WiFi

  To run this example define the platformio.ini as below.

*/

#include <VS1053.h>
#include <ESP_WiFiManager.h>
//OTA
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//#include "configFile.h"
// partie ecran
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);




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

void testscrolltext(void) {
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("0x")); 
  display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

void display_str(const char *str){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  display.println(str);
  display.display();
  display.clearDisplay();

}


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
        display_str ("Air spectrum");
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
        display_str ("RTL 2");
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
        display_str ("Hit West");
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
    ESP_WiFiManager wm ("radioseb");
     
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

   //OTA
   // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
   ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());



   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
 display_str ("Hello !!!");

}

void loop() {
  //OTA
  ArduinoOTA.handle();

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
