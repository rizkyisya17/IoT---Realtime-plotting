var stream

/* const express = require('express'),
      app = express(),
      SerialPort = require('serialport'),
      port = new SerialPort('COM3', {
    baudRate: 9600,
}, err => {
    if(err != null) {
        console.log(err)
        return
    }
})



app.get('/data', (req,res) => {
    port.setMaxListeners(500)
    port.on('data', data => {
        data = JSON.stringify(data)
        data = JSON.parse(data)
        stream = String.fromCharCode.apply(String, data.data).replace(/\0\r\n/g,'');
        stream = stream.split("?")
        console.log(stream)
        if (stream.length > 6){
        stream = {
            "lat_kapal" : parseFloat(stream[0]),
            "lng_kapal" : parseFloat(stream[1])
        }}
	})	
    res.json(stream)
}) */

const express = require('express'),
      app = express(),
      serialport = require('serialport'),
	  SerialPort = serialport.SerialPort,
      sp = new serialport('COM3', {
    baudRate: 57600,
}, err => {
    if(err != null) {
        console.log(err)
        return
    }
})


app.get('/data', (req,res) => {
	sp.setMaxListeners(9000)
    const Readline = serialport.parsers.Readline;
	const parser = new Readline;
	sp.pipe(parser);
	parser.on('data',onData);
	res.json(stream)
})

function onData(value){
/* console.log(value); */
autoGenerateVarFromArray(value.split(' '), 'temp');
stream = {
            "lat_kapal" : parseFloat(temp_0),
            "lng_kapal" : parseFloat(temp_1)
        }
//console.log(stream)		
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

const cport = 3000
app.use(express.static(__dirname))
app.get('/', (req,res) => res.sendFile(__dirname + '/public/index.html'))
app.listen(cport, () => console.log(`Listening on port ${cport}!`))

