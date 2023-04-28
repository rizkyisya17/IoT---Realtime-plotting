// Membaca data dari serial port
// pada Arduino
 
var serialport = require("serialport");
var SerialPort = serialport.SerialPort;
var portName = process.argv[2];
 
var sp = new serialport("COM3",{
baudRate: 9600
});
const Readline = serialport.parsers.Readline;
const parser = new Readline;
sp.pipe(parser);
 
parser.on('data',onData);

 
function onData(data){
/* console.log(data); */
autoGenerateVarFromArray(data.split(' '), 'temp');
string = parseFloat(temp_1)
console.log(string);

/* document.getElementById("lat_kapal").innerHTML = temp_1; */
/* document.getElementById("lng_kapal").innerHTML = temp_3; */
}

function autoGenerateVarFromArray(srcArray, varNamePrefix)
{
  var i = 0
  while(i < srcArray.length)
  {
    this[varNamePrefix +'_' + i] = srcArray[i]; 
    i++;
  } 
}