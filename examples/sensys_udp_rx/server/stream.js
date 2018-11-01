const fs = require('fs');
const stream = require('stream');
const SerialPort = require('serialport');

function createMyStream(){
  const magic = Buffer.from([0x80, 0x81]);
  const header_len = magic.length + 16 + 2;
  let address = null;
  let bytes_left = 0;
  let payload_len = 0;
  let buffer = Buffer.from('');

  return new stream.Writable({
    writableObjectMode: true,
    write: function(chunk, encoding, callback) {
      buffer = Buffer.concat([buffer, chunk]);
      while (true) {
        if (bytes_left == 0) {
          let m = buffer.indexOf(magic);
          if (m >= 0 && buffer.length - m >= header_len) {
            buffer = buffer.slice(m + magic.length);
            address = buffer.slice(0, 16);
            buffer = buffer.slice(16);
            payload_len = buffer.readUInt16BE(0);
            buffer = buffer.slice(2);
            bytes_left = payload_len - buffer.length;
          } else {
            buffer = buffer.slice(-(magic.length - 1));
            break;
  	  }
        } else if (buffer.length >= payload_len) {
            let payload = buffer.slice(0, payload_len);
            buffer = buffer.slice(payload_len);
            bytes_left = 0;
            let result = {
              address: address,
              payload: JSON.parse(payload)
            };
            this.emit("packet", result);
        } else {
          bytes_left = payload_len - buffer.length;
          break;
        }
      }
      callback();
    }
  });
}

if (process.argv.length < 3) {
  console.log("Usage: node stream.js [serial-device], e.g. node stream.js /dev/ttyUSB0");
  process.exit(1);
}

const port = process.argv[2];
const serialport = new SerialPort(port);

const mystream = createMyStream();
serialport.pipe(mystream);

let recent_data = {};

mystream.on('packet', (packet) => {
  let address_last2 = packet.address.slice(-2).readUInt16BE().toString(16);
  recent_data[address_last2] |= {};
  let slot = recent_data[address_last2];
  slot.timestamp = new Date().valueOf();
  for (k in packet.payload) {
    slot[k] = packet.payload[k];
  }
  console.log(packet);
});

const express = require('express');
      serveStatic = require('serve-static');
      app = express();
      web_port = 3000;

app.use(serveStatic('static'));
app.get('/data', (req, res) => res.json(recent_data));
app.get('/data/:addr', (req, res) => res.json(recent_data[req.params.addr]));

app.listen(web_port, () => console.log(`Web app listening on port ${web_port}!`));

