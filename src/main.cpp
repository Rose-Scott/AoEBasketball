#include <Arduino.h>
#include <Adafruit_LEDBackpack.h>
#include <Wire.h>

#define SENSORPIN 4

// score and time variables
int sensorState = 0, lastState = 0 , score = 0;

int timeLimit = 59, timeLeft = timeLimit; 
unsigned long timeStart;
Adafruit_7segment display = Adafruit_7segment();


void setup() {
  // initialize the sensor pin as an input:
  pinMode(SENSORPIN, INPUT_PULLUP);     
  lastState = digitalRead(SENSORPIN);

  // Prepare the display
  timeStart = millis();
  display.begin(0x70);
  display.clear();
  updateDisplay();
  
  Serial.begin(9600);
}

void loop() {
  if (timeLeft > 0) {
    scoring();
    timer();
  }
}

void scoring()  {
  // read the state of the pushbutton value:
  sensorState = digitalRead(SENSORPIN);

  // check if the sensor beam is broken and it is the first iteration where it is broken
  // TODO may cause error in which case we add debounce
  if (sensorState == LOW && sensorState != lastState) {
    score++;
    updateDisplay()
  }
  lastState = sensorState;
}

void timer()  {
  unsigned long now = millis();
  int elapsedSeconds = (now - timeStart) / 1000;

  if (timeLimit - elapsedSeconds != timeLeft) {
    timeLeft = timeLimit - elapsedSeconds;
    updateDisplay();
  }
}

void updateDisplay() {
  display.writeDigitNum(0, score / 10);
  display.writeDigitNum(1, score % 10);

  display.writeDigitNum(3, timeLeft / 10);
  display.writeDigitNum(4, timeLeft % 10);

  display.writeDisplay();
}