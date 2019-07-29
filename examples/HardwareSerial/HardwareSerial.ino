#include "PixelblazeSensorBoard.h"

/*
 * NOTE: many older Arduinos have only a single serial port.
 * Often this serial port being driven by a usb serial adapter and used for uploads.
 * If you have such a board, use the SoftwareSerial example.
*/
#define SENSOR_SERIAL Serial1
PixelblazeSensorBoard sensorBoard(SENSOR_SERIAL);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting up!");
  
  //the sensor board outputs data at 115200 baud
  SENSOR_SERIAL.begin(115200);
}

unsigned long timer = 0;
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
