#include "PixelblazeSensorBoard.h"
#include <SoftwareSerial.h>
#include <Ticker.h>

/*
 * NOTE: on most esp8266 boards the second serial port RX is unavailable.
 * Often the first serial port being driven by a usb serial adapter.
 * If you have a bare esp8266, you can use the hardware serial port.
*/
static const int RXPin = 12;
SoftwareSerial softwareSerial(RXPin, -1, false, 128);

PixelblazeSensorBoard sensorBoard(softwareSerial);
Ticker ticker;

void ICACHE_RAM_ATTR processData() {
  //this will read and process all buffered data, but won't wait for more
  sensorBoard.readAvailable();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting up!");

  //the sensor board outputs data at 115200 baud
  softwareSerial.begin(115200);
  //process data in an interrupt every 8ms
  ticker.attach_ms(8, processData);
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
