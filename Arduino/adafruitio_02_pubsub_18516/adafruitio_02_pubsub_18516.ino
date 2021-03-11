// Adafruit IO Publish & Subscribe Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/*** Configuration ****/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/*** Example Starts Here ****/

// this int will hold the current count for our sketch
int enviar = 0;

#define RXD2 16
#define TXD2 17
// Track time of last published messages and limit feed->save events to once
// every IO_LOOP_DELAY milliseconds.
//
// Because this sketch is publishing AND subscribing, we can't use a long
// delay() function call in the main loop since that would prevent io.run()
// from being called often enough to receive all incoming messages.
//
// Instead, we can use the millis() function to get the current time in
// milliseconds and avoid publishing until IO_LOOP_DELAY milliseconds have
// passed.
#define IO_LOOP_DELAY 5000
unsigned long lastUpdate = 0;

// set up the 'counter' feed
AdafruitIO_Feed *enviarFeed = io.feed("enviar");
AdafruitIO_Feed *recibirFeed = io.feed("recibir");
AdafruitIO_Feed *LED2Feed = io.feed("LED2");

void setup() {
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  // start the serial connection
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(1000);

  // wait for serial monitor to open
  while (! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  recibirFeed->onMessage(handleMessage);
  LED2Feed->onMessage(handleMessage2);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  recibirFeed->get();
  LED2Feed->get();

}

void loop() {     
   if(Serial2.available()> 0){
     //Serial.write(Serial2.read());
     enviar = Serial2.read();
     Serial.write(Serial2.read());  
   }

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  
  //enviar=random(0,100);
  if (millis() > (lastUpdate + IO_LOOP_DELAY)) {
    // save count to the 'counter' feed on Adafruit IO
    Serial.print("Valor de PIC -> ");
    Serial.println(enviar);
    enviarFeed->save(enviar);

    // increment the count by 1
    //count++;

    // after publishing, store the current time
    lastUpdate = millis();
  }
  delay(3000);
  
}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received led1 <- ");
  Serial.println(data->value());

  if (data->toString() == "ON") {
    digitalWrite(23, HIGH);
  }
  if (data->toString() == "OFF") {
    digitalWrite(23, LOW);
  }
}
void handleMessage2(AdafruitIO_Data *data) {

  Serial.print("received led2 <- ");
  Serial.println(data->value());

   if(data->toString() == "ON"){   
    digitalWrite(22, HIGH);
    
    }
   
   if(data->toString() == "OFF"){ 
    digitalWrite(22, LOW);  
    }
}
