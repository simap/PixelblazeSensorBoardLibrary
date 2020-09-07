//NOTE: interrupts may cause issues with fastLED and WS2812 - see the HardwareSerial example
#include "PixelblazeSensorBoard.h"

#define SENSOR_SERIAL Serial1
PixelblazeSensorBoard sensorBoard(SENSOR_SERIAL);
IntervalTimer myTimer;

void processData() {
  //this will read and process all buffered data, but won't wait for more
  sensorBoard.readAvailable();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting up!");

  //the sensor board outputs data at 115200 baud
  SENSOR_SERIAL.begin(115200);
  //process data in an interrupt every 4ms
  myTimer.begin(processData, 4000); 
}

void loop() {
  Serial.print("Last sensor reading was ");
  Serial.print(sensorBoard.dataAgeMillis());
  Serial.print("ms ago energyAverage:");
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

  delay(500);
}
