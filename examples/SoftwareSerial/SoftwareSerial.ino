#include "PixelblazeSensorBoard.h"
#include <SoftwareSerial.h>

/*
 * NOTE: many older Arduinos have only a single serial port.
 * Often this serial port being driven by a usb serial adapter and used for uploads.
 * If you have a board with another serial port, use the HardwareSerial example.
*/
static const int RXPin = 4, TXPin = 3;
SoftwareSerial softwareSerial(RXPin, TXPin);
PixelblazeSensorBoard sensorBoard(softwareSerial);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting up!");
  
  //the sensor board outputs data at 115200 baud
  softwareSerial.begin(115200);
}

void loop() {
  unsigned long start = millis();
  if (sensorBoard.readNextFrame()) {
    unsigned long end = millis();
    Serial.print("Got frame in ");
    Serial.print(end - start);
    Serial.print("ms energyAverage:");
    Serial.print(sensorBoard.energyAverage());
    Serial.print(" light:");
    Serial.print(sensorBoard.light());
    Serial.print(" X:");
    Serial.print(sensorBoard.accelerometerX());
    Serial.print(" Y:");
    Serial.print(sensorBoard.accelerometerY());
    Serial.print(" Z:");
    Serial.print(sensorBoard.accelerometerZ());
    Serial.print(" maxFrequencyMagnitude:");
    Serial.print(sensorBoard.maxFrequencyMagnitude());
    Serial.print(" maxFrequency:");
    Serial.print(sensorBoard.maxFrequency());
    Serial.println();
  }
}
