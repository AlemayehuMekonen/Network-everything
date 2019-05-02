/*
	UDP And HTTP Server
	By Alemayehu Abebe
*/

/* UDP server talks to Arduino */
var dgram = require('dgram');
var arduinoUDPServer = dgram.createSocket('udp4')
var MY_PORT_FOR_ARDUINO = 7000;
var ARDUINO_PORT_FOR_ME= 5000;
var ARDUINO_IP_ADDRESS = '192.168.1.9'; 

/* HTTP server talks to browser */
const http = require('http')
const express = require('express'); // web server application
const app = express();        // instantiate express server
const httpServer = http.Server(app);  // connects http library to server
const HTTP_SERVER_PORT = 8000; 

// Express creates the simple web page
// The argument says where to find pages and scripts
app.use(express.static('public'));  

// websockets so that webpage can talk back to server
const webSocket = require('socket.io')(httpServer);  

/* Arduino UDP server callback functions */

function ArduinoUDPServerIsListening() {
	console.log('Arduino UDP Server is listening');
}

function ArduinoUDPServerReceivedMessage(message, sender) {

	// If the message is a byte we need to read a byte

	if (message.readUInt8(0) == 1 ) {
		console.log( "received a 1");
		webSocket.emit('greenbutton', 1);
	}

	if (message.readUInt8(1) == 1 ) {
		console.log( "received a 1");
		webSocket.emit('bluebutton', 2);
	}

	if (message.readUInt8(2) == 1 ) {
		console.log( "received a 1");
		webSocket.emit('yellowbutton', 3);
	}

	if (message.readUInt8(0) == 0 && message.readUInt8(1) == 0 && message.readUInt8(2) == 0 ) {
		console.log( "all are off");
		// Now send a message to the web browser to change color
		webSocket.emit('LEDS are off', 0);
	}
}

/* Register the UDP callback functions */
arduinoUDPServer.bind( MY_PORT_FOR_ARDUINO );
arduinoUDPServer.on( 'listening', ArduinoUDPServerIsListening);
arduinoUDPServer.on( 'message', ArduinoUDPServerReceivedMessage);

/* HTTP callback functions */

httpServer.listen(HTTP_SERVER_PORT, () => {
	console.log('web server: Listening at', httpServer.address());
});

httpServer.on('connection', () => {
  console.log("web server: An HTTP client has connected")
});


// Websocket event handler 
// for UDP messages coming from the browser

webSocket.on('connect', (socket) => {
	// array for the message
	// newLEDState[0] = LED number 
	// newLEDState[1] = LED state 
	const SIZEOF_LED_DATA = 2;
	var newLEDState = new Uint8Array(SIZEOF_LED_DATA ); 
  
	console.log('Web server socket: Client connected');

  socket.on('redLEDOn', () => {
		console.log('HAPPY');
		newLEDState[0] = 0;  // Red is LED number 0
		newLEDState[1] = 1; // turn on the LED
    // Send the message to Arduino
    arduinoUDPServer.send(
			newLEDState,
			0, // offset to the message start in the buffer
			SIZEOF_LED_DATA,
			ARDUINO_PORT_FOR_ME, 
			ARDUINO_IP_ADDRESS);
  });

  socket.on('redLEDOff', () => {
		console.log('SAD');
		newLEDState[0] = 0;  // Red is LED number 0
		newLEDState[1] = 0; // turn off the LED
    // Send the message to Arduino
    arduinoUDPServer.send(
			newLEDState,
			0, // offset to the message start in the buffer
			SIZEOF_LED_DATA,
			ARDUINO_PORT_FOR_ME, 
			ARDUINO_IP_ADDRESS);
  });

  socket.on('greenLEDOn', () => {
		console.log('ANGRY');
		newLEDState[0] = 1;  // green is LED number 1
		newLEDState[1] = 1; // turn on the LED
    // Send the message to Arduino
    arduinoUDPServer.send(
			newLEDState,
			0, // offset to the message start in the buffer
			SIZEOF_LED_DATA,
			ARDUINO_PORT_FOR_ME, 
			ARDUINO_IP_ADDRESS);
  });

  socket.on('greenLEDOff', () => {
		console.log('EXCITED');
		newLEDState[0] = 1;  // green is LED number 1
		newLEDState[1] = 0; // turn off the LED
    // Send the message to Arduino
    arduinoUDPServer.send(
			newLEDState,
			0, // offset to the message start in the buffer
			SIZEOF_LED_DATA,
			ARDUINO_PORT_FOR_ME, 
			ARDUINO_IP_ADDRESS);
  });

  socket.on('blueLEDOn', () => {
		console.log('STRESSED');
		newLEDState[0] = 2;  // blue is LED number 2
		newLEDState[1] = 1; // turn on the LED
    // Send the message to Arduino
    arduinoUDPServer.send(
			newLEDState,
			0, // offset to the message start in the buffer
			SIZEOF_LED_DATA,
			ARDUINO_PORT_FOR_ME, 
			ARDUINO_IP_ADDRESS);
  });

  socket.on('blueLEDOff', () => {
		console.log('SCARED');
		newLEDState[0] = 2;  // blue is LED number 2
		newLEDState[1] = 0; // turn off the LED
    // Send the message to Arduino
    arduinoUDPServer.send(
			newLEDState,
			0, // offset to the message start in the buffer
			SIZEOF_LED_DATA,
			ARDUINO_PORT_FOR_ME, 
			ARDUINO_IP_ADDRESS);
  });

  // if you get the 'disconnect' message, say the user disconnected
  socket.on('disconnect', () => {
    console.log('Web server socket: user disconnected');
  });
});
