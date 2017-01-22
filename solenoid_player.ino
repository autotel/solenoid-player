#include <TimerOne.h>
unsigned int beatlong = 0x00FF;
unsigned int currentloop = 0;
int pinTimers [] = {
  0, 0, 0, 0, 0, 0, 0, 0
};
unsigned int pinDurations [] = {
  90,
  90,
  10
};

int pins [] = {
  12, 11, 10, 9, 8, 7, 6
};
//wether to apply analog write to the pin to represent the note
boolean pinTone[] = {false, false, true};
//wether to wait for a note off to stop giving voltage
boolean pinSustained[] = {false, false, true};
int pinAmount = 8;
void setup() {
  Serial.begin(115200);
  Serial.write(0xFA);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(pinTimer); // blinkLED to run every 0.15 seconds
}
void loop() {
  if (currentloop > 0) {
    currentloop--;
  }
  else {
    //Serial.write(0xF8);
    currentloop = beatlong;
  }

  //pinTimer();
  midiCheck();
}
void midiCheck() {
  unsigned int inByte = 0;
  unsigned int inByte2 = 0;
  unsigned int inByte3 = 0;
  if (Serial.available() > 2) {
    inByte = Serial.read();//read first byte
    inByte2 = Serial.read();//read next byte
    inByte3 = Serial.read();//read final byte
    //trig(5, 0, 0);
  }
  //if is note on
  if ((inByte & 0xF0) == 0x90) {
    trig(inByte & 0xF, inByte2, inByte3);
  }
  //if is note off

  if ((inByte & 0xF0) == 0x80) {
    untrig(inByte & 0xF, inByte2, inByte3);
  }


  //  switch (inByte) {
  //    case 0x90:
  //      trig(0, inByte2, inByte3);
  //      break;
  //    case 0x91:
  //      trig(1, inByte2, inByte3);
  //      break;
  //    case 0x92:
  //      trig(2, inByte2, inByte3);
  //      break;
  //    case 0x93:
  //      trig(3, inByte2, inByte3);
  //      break;
  //    case 0x94:
  //      trig(4, inByte2, inByte3);
  //      break;
  //    case 0x95:
  //      trig(5, inByte2, inByte3);
  //      break;
  //    case 0x96:
  //      trig(6, inByte2, inByte3);
  //      break;
  //    case 0x97:
  //      trig(7, inByte2, inByte3);
  //    default:
  //      trig(0, 0, 0);
  // }
}
void pinTimer() {
  for (int a = 0; a < pinAmount; a++) {
    //if (pinTimers[a] == 1) {

    // }
    if (pinTimers[a] > 1) {
      if (!pinSustained[a])
        pinTimers[a]--;
    } else {
      digitalWrite(pins[a], LOW);
      pinTimers[a]--;
    }
  }
}
void trig(unsigned int a, unsigned int b, unsigned int c) {
  if (pinSustained[a]) {
    pinTimers[a] = 10;
  } else {
    pinTimers[a] = (pinDurations[a] * c) / 288;
  }
  
  if (pinTone[a]) {
    analogWrite(pins[a], b * 10);
  } else {
    digitalWrite(pins[a], HIGH);
  }
}
void untrig(unsigned int a, unsigned int b, unsigned int c) {
  if (pinSustained[a])
    pinTimers[a] = 0;
}
