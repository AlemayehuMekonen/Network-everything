/*

  WiFi UDP Send Byte on Button Press
  Based on UDP Send and Receive String

  created 3 February 2019
  by Alemayehu and Henok
*/

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 2390;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet

WiFiUDP Udp;

const int GREEN_PIN = 3; // transmitting
const int YELLOW_PIN = 4;
const int BLUE_PIN = 5;

const int blue = 6; // receiving
const int yellow = 7;
const int green= 8;


// remember the button state so we only send
// when the state changes
boolean buttonStategreen;
boolean buttonStateblue;
boolean buttonStateyellow;
boolean lastButtonStategreen = LOW; // arbitrary
boolean lastButtonStateblue = LOW;
boolean lastButtonStateyellow = LOW;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();

  Serial.print("Initializing WiFiUDP library and listening on port ");
  Serial.println(localPort);
  Udp.begin(localPort);
  pinMode(blue, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
}

void loop() {

  // IP address of the receiving device
  IPAddress receivingDeviceAddress(192, 168, 1, 24);
  unsigned int receivingDevicePort = 5000;

  receive(); //checks if there is an incoming signal from transmitter
//  unsigned int localPort = 5000;
  buttonStategreen = digitalRead(GREEN_PIN);
  buttonStateblue = digitalRead(BLUE_PIN);
  buttonStateyellow = digitalRead(YELLOW_PIN);

  if (buttonStategreen != lastButtonStategreen ||buttonStateyellow != lastButtonStateyellow||buttonStateblue != lastButtonStateblue) {

    Serial.println("button state changed; sending new state");
    Udp.beginPacket(receivingDeviceAddress, receivingDevicePort);
    Udp.write(buttonStategreen);
    Udp.write(buttonStateyellow);
    Udp.write(buttonStateblue);
    Udp.endPacket();

    lastButtonStategreen = buttonStategreen;
    lastButtonStateblue = buttonStateblue;
    lastButtonStateyellow = buttonStateyellow;
    
  }

  // once we send a packet to the server, it might
  // respond, so read it

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
//    if (len > 0) packetBuffer[len] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);


  }
}

void receive()
{
//  unsigned int receivingDevicePort = 5000;
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();

  // if nobody sent us anything, packetSize
  // would be zero, so a non-zero packetSize
  // means we have something to work on
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);

    // We are expecting exactly one byte,
    // and that byte tells us whether to turn
    // the LED on or off. There is no need to
    // inspect the byte, simply send it directly
    // to digitalWrite()
//    digitalWrite(3, packetBuffer[0]);
//    digitalWrite(4, packetBuffer[1]);
//    digitalWrite(5, packetBuffer[2]); 
     int blueSwitchState = packetBuffer[0];
     int redSwitchState = packetBuffer[1];
     int greenSwitchState = packetBuffer[2];
     digitalWrite(blue, blueSwitchState);
     digitalWrite(yellow, redSwitchState);
     digitalWrite(green, greenSwitchState);


 
    // if we wanted to send anything back e.g. to
    // acknowledge the packet
    // this would be the place

  
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
