//
// Created by benjamin on 16.11.19.
//

//
// This program simulates the latch and clock signal of the snes gaming console.
// The frequence is not as high as the one of the real snes.
// This was used to initially test the program that simulates the controller.
//

#define clockpin 14
#define latch 12

#include "Arduino.h"

void setup(){
    pinMode(clockpin, OUTPUT);
    pinMode(latch, OUTPUT);
}

void loop(){

    digitalWrite(latch, HIGH);
    delayMicroseconds(1000);
    digitalWrite(latch, LOW);
    for (int i = 0; i < 16; i++) {
        delayMicroseconds(500);
        digitalWrite(clockpin, LOW);
        delayMicroseconds(500);
        digitalWrite(clockpin, HIGH);
    }
    delayMicroseconds(20000);

}
