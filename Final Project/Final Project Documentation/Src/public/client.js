
/* 
This is the websocket library which will allow us to send messages
back to the web server 
Alemayehu Abebe
*/
var socket = io();



function redLEDOn() {
  socket.emit('redLEDOn'); 
  var audio = document.getElementById("audio");
  audio.play();
}

function redLEDOff() {
  socket.emit('redLEDOff');
  audio.pause();
}

