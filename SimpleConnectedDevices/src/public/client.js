/* 
This is the websocket library which will allow us to send messages
back to the web server 
Alemayehu Abebe
*/
var socket = io();

socket.on('greenbutton', (data) => {
	console.log('received data from webserver: ' + data);
	image1();
});

socket.on('bluebotton', (data) => {
	console.log('received button pressed event from webserver: ' + data);
	image2();
});

socket.on('yellowbutton', (data) => {
	console.log('received button released from webserver: ' + data);
	image3();
});

socket.on('off', (data) => {
	console.log('received button released from webserver: ' + data);
	hideTheImage();
});
function image1 () {

	var audio = document.getElementById("audio");

	var placeholder = document.getElementById("placeholder");

	placeholder.src = "paradize.jpg"; // put images in public folder

	placeholder.style.opacity = 100;

	audio.play();
}


function image2 () {

	var audio = document.getElementById("audio");

	var placeholder = document.getElementById("placeholder");

	placeholder.src = "sadrelief.jpg"; // put images in public folder

	placeholder.style.opacity = 100;

	audio.play();
}

function image3 () {

	var audio = document.getElementById("audio");

	var placeholder = document.getElementById("placeholder");

	placeholder.src = "stressrelief.jpg"; // put images in public folder

	placeholder.style.opacity = 100;
	audio.play();
}
function hideTheImage () {
	var placeholder = document.getElementById("placeholder");

	placeholder.style.opacity = 0;
}

function redLEDOn() {
  socket.emit('redLEDOn'); 
}

function redLEDOff() {
  socket.emit('redLEDOff');
}

function greenLEDOn() {
  socket.emit('greenLEDOn'); 
}

function greenLEDOff() {
  socket.emit('greenLEDOff');
}

function blueLEDOn() {
  socket.emit('blueLEDOn'); 
}

function blueLEDOff() {
  socket.emit('blueLEDOff');
}
