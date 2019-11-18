//
// Created by benjamin and timo on 16.11.19.
//

//
// This program accepts button configurations send over serial port. If connected
// to a snes, it will simulate a snes controller and transmit the button configuration
// that was recieved over serial.
//

#define clockpin 5
#define latchpin 4
#define datapin 19

#include "Arduino.h"

/*!
   Interrupt Routines
*/
bool dataarr[16]; // This holds the current button configuration, 1 means the button is *not* pressed.
byte datafrompi[2]; // this seems to be legacy code
int bitcounter = 0; // a counter variable to handle what button setting to be 
                    // send next.

int letchCounter = 0; // legacy?

// The transmission of the current button configuration will be started after a
// latch signal was detected, according to the timing protocol the first button
// setting will be transmitted here.
void latchdetected() {
  bitcounter = 0;
  digitalWrite(datapin, dataarr[bitcounter]);
}

// On every rising clock signal the next button setting will be transmitted.
// 16 Button setting will be transmitted in total, even though the last four
// are not used by the snes.
void sendrestofdata() {
  bitcounter++; // Increase buttencounter to send the next signal.

  digitalWrite(datapin, dataarr[bitcounter]);

  // after sendig all 16 button settings the counter is reset and the data
  // signal is reset to low.
  if (bitcounter > 15) {
    bitcounter = 0;
    //delayMicroseconds(5);
    digitalWrite(datapin, LOW);
  }
}

// Converts an ascii character recieved over serial to the corresponding int value.
int convertASCIIToInt(int charInt) {
  return charInt - 48;
}

/*!
   Setup
*/
void setup() {

  /*!
     Setting Pinmodes
  */
  pinMode(clockpin, INPUT); // We need to read the snes's clock signal.
  pinMode(latchpin, INPUT); // We also need the read its latch signal.
  pinMode(datapin, OUTPUT); // The data pin is used to transmit the button configuration.

  /*!
     Attaching Interups to clock- and latchpin
  */
  //Time to start the Transmission.
  attachInterrupt(digitalPinToInterrupt(latchpin), latchdetected, RISING);
  //Detecting clocks for the rest of the databits.
  attachInterrupt(digitalPinToInterrupt(clockpin), sendrestofdata, RISING);

  /*!
     Initializing Serial Interface
  */
  Serial.begin(115200);
}

void loop() {

  // The following comment is legacy code and could be removed.
  /*!
     Receiving data from raspberry pi
  */
  /*if (Serial.available() > 0) {
    for (int i = 0; i < 2; i++) {
      int charInt = Serial.read();
      if (!isDigit(charInt)) {
        datafrompi[i] = 0b00000000;
      } else {
        datafrompi[i] = pow(2, convertASCIIToInt(charInt));
      }
    }
    Serial.flush();
    }*/

  /*!
     Decoding data from raspberry pi
    /*
    for (int i = 0; i < 2; i++) {
    for (int j = 7; j >= 0; j--) {
      dataarr[j + i * 8] = !bitRead(datafrompi[i], j);
    }
    }*/

  // For the current button configuration we expect a string of 16 characters
  // that tells us which buttons are pressed.
  // Example: "1000000000000000" means that 'b' is pressed.
  // We only expect to recieve valid input over serial, that is a 16 character
  // string consisting of 1 and 0.
  if (Serial.available() > 0) {
    for ( int i = 0; i < 16; i++) {
      int charInt = Serial.read();
      int in = convertASCIIToInt(charInt);

      if (in == 1) {
        dataarr[i] = false;
      } else {
        dataarr[i] = true;
      }
    }
    Serial.flush(); // we do not want to read more than 16 characters.
                    // Especially \n and such are omitted.
  }
}
