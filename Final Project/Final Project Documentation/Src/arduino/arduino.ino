/*

  WiFi UDP device responding to buttons on a web page 
  sent from a web page to node.js client to node.js server
  and finally to here

  Alemayehu Abebe

*/

#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

//
//This is a sample code from internet to implement the neopixel 

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

#define NUM_LEDS 7

#define BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

unsigned int myPort = 5000;      // local port to listen on
unsigned int remoteServerPort = 7000;  // remote port to talk to
// IPAddress remoteServerAddress(192, 168, 1, 5); // server at home
//IPAddress remoteServerAddress(192, 168, 1, 21); 
IPAddress remoteServerAddress(10,225,96,197);

char packetBuffer[255]; //buffer to hold incoming packet

WiFiUDP Udp;


const int fingertip1 = 1; // transmitting
const int fingertip2 = 2;
const int fingertip3 = 3;
const int fingertip4 = 4;
const int fingertip5 = 5;



// remember the button state so we only send
// when the state changes
boolean buttonStategreen;
boolean buttonStateblue;
boolean buttonStateyellow;
boolean lastButtonStategreen = LOW; // arbitrary
boolean lastButtonStateblue = LOW;
boolean lastButtonStateyellow = LOW;

String LED_ON_MESSAGE = "ledON";
String LED_OFF_MESSAGE = "ledOFF";

// remember the button state so we only send
// when the state changes

void setup() {

  
//  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
//  #if defined (__AVR_ATtiny85__)
//    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
//  #endif
//  // End of trinket special code
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  pinMode(fingertip1, OUTPUT);
  pinMode(fingertip2, OUTPUT);
  pinMode(fingertip3, OUTPUT);
  pinMode(fingertip4, OUTPUT);
  pinMode(fingertip5, OUTPUT);
  
  // Set all actuator pins as outputs
  
  //Initialize serial
  Serial.begin(9600);
  // wait for serial port to connect. Needed for native USB port only
  // Remove this if running without a serial monitor
  //while (!Serial) {
   // ; 
  //}

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(5000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();

  Serial.print("Initializing WiFiUDP library and listening on port ");
  Serial.println(myPort);
  Udp.begin(myPort);
}

void loop() {
  
  // It's polite to listen first
  // Did the server send us anything?
  int packetSize = Udp.parsePacket(); 
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.print(packetSize);
    Serial.print(" from address ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);

    // Activate the actuators as requested

    if (packetBuffer[1]== 0)
    {
     digitalWrite(fingertip1, LOW); 
     digitalWrite(fingertip2, LOW);
     digitalWrite(fingertip3, LOW);
     digitalWrite(fingertip4, LOW); 
     digitalWrite(fingertip5, LOW);
     pulseWhite(5);
     
    }
    
    if (packetBuffer[1]== 1)
    {
     digitalWrite(fingertip1, HIGH);
     Serial.print("High"); 
     digitalWrite(fingertip2, HIGH);
     digitalWrite(fingertip3, HIGH);
     digitalWrite(fingertip4, HIGH); 
     digitalWrite(fingertip5, HIGH);
     fullRed();
//     fire.Draw();
     delay(random(50,150));
     colorWipe(strip.Color(255, 0, 0), 50); // Red
    }
    
    
//
//  buttonStategreen = digitalRead(GREEN_PIN);
//  buttonStateblue = digitalRead(BLUE_PIN);
//  buttonStateyellow = digitalRead(YELLOW_PIN);
//
//  if (buttonStategreen != lastButtonStategreen ||buttonStateyellow != lastButtonStateyellow||buttonStateblue != lastButtonStateblue) {
//
//    Serial.println("button state changed; sending new state");
//    Udp.beginPacket(remoteServerAddress, remoteServerPort);
//    Udp.write(buttonStategreen);
//    Udp.write(buttonStateyellow);
//    Udp.write(buttonStateblue);
//    Udp.endPacket();
//
//    lastButtonStategreen = buttonStategreen;
//    lastButtonStateblue = buttonStateblue;
//    lastButtonStateyellow = buttonStateyellow;
//    
//  }
}
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("My IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    //delay(wait);
  }
}

void pulseWhite(uint8_t wait) {
  for(int j = 0; j < 256 ; j++){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0,0,0, neopix_gamma[j] ) );
        }
        //delay(wait);
        strip.show();
      }

  for(int j = 255; j >= 0 ; j--){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0,0,0, neopix_gamma[j] ) );
        }
        //delay(wait);
        strip.show();
      }
}

//
//void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops) {
//  float fadeMax = 100.0;
//  int fadeVal = 0;
//  uint32_t wheelVal;
//  int redVal, greenVal, blueVal;
//
//  for(int k = 0 ; k < rainbowLoops ; k ++){
//    
//    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel
//
//      for(int i=0; i< strip.numPixels(); i++) {
//
//        wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);
//
//        redVal = red(wheelVal) * float(fadeVal/fadeMax);
//        greenVal = green(wheelVal) * float(fadeVal/fadeMax);
//        blueVal = blue(wheelVal) * float(fadeVal/fadeMax);
//
//        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );
//
//      }
//
//      //First loop, fade in!
//      if(k == 0 && fadeVal < fadeMax-1) {
//          fadeVal++;
//      }
//
//      //Last loop, fade out!
//      else if(k == rainbowLoops - 1 && j > 255 - fadeMax ){
//          fadeVal--;
//      }
//
//        strip.show();
//        //delay(wait);
//    }
//  
//  }
//
//
//
//  //delay(500);
//
//
//  for(int k = 0 ; k < whiteLoops ; k ++){
//
//    for(int j = 0; j < 256 ; j++){
//
//        for(uint16_t i=0; i < strip.numPixels(); i++) {
//            strip.setPixelColor(i, strip.Color(0,0,0, neopix_gamma[j] ) );
//          }
//          strip.show();
//        }
//
//        //delay(2000);
//    for(int j = 255; j >= 0 ; j--){
//
//        for(uint16_t i=0; i < strip.numPixels(); i++) {
//            strip.setPixelColor(i, strip.Color(0,0,0, neopix_gamma[j] ) );
//          }
//          strip.show();
//        }
//  }
//
//  //delay(500);
//
//
//}

void fullRed() {


   for(int i=0; i<strip.numPixels(); i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    strip.setPixelColor(i, strip.Color(80, 35, 0));

    strip.show();   // Send the updated pixel colors to the hardware.

    delay(5); // Pause before next pass through loop
  }
}
  
//    for(uint16_t i=0; i<strip.numPixels(); i++) {
//        strip.setPixelColor(i, strip.Color(255,255,0 ) );
//    }
//      strip.show();
//      delay(500);
//}
//void colorWipe(uint32_t c, uint8_t wait) {
//  for(uint16_t i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, c);
//      strip.show();
//      delay(wait);
//  }
//}


// Slightly different, this makes the rainbow equally distributed throughout
//void rainbowCycle(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
//    for(i=0; i< strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
//    }
//    strip.show();
//    //delay(wait);
//  }
//}



uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}
