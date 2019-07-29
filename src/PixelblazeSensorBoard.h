//
// Created by Ben Hencke on 2019-07-28.
//

#ifndef SB_ARDUINO_PIXELBLAZESENSORBOARD_H
#define SB_ARDUINO_PIXELBLAZESENSORBOARD_H

#include <Arduino.h>


#ifndef ICACHE_RAM_ATTR
#define ICACHE_RAM_ATTR
#endif

typedef struct {
//    char header[6]; // "SB1.0\0"
    uint16_t frequencyData[32];
    uint16_t energyAverage;
    uint16_t maxFrequencyMagnitude;
    uint16_t maxFrequency; //in hz
    int16_t accelerometer[3];
    uint16_t light;
    uint16_t analogInputs[5];
//    char end[4]; // "END\0"
} __attribute__((__packed__)) SB10Frame;


class PixelblazeSensorBoard {
public:
    Stream &source;
    enum {HEADER, DATA, END} mode = HEADER;
    uint8_t pos = 0;
    SB10Frame sb10Frame;
    uint8_t frameBuffer[sizeof(SB10Frame)];
    unsigned long lastFrameTime = 0;

    PixelblazeSensorBoard(Stream &source) : source(source) {
    }

    uint16_t frequencyData(int index) {
        if (index < 0 || index > 31)
            return 0;
        return sb10Frame.frequencyData[index];
    }
    uint16_t energyAverage() {
        return sb10Frame.energyAverage;
    }
    uint16_t maxFrequencyMagnitude() {
        return sb10Frame.maxFrequencyMagnitude;
    }
    uint16_t maxFrequency() {
        return sb10Frame.maxFrequency;
    }
    int16_t accelerometerX() {
        return sb10Frame.accelerometer[1];
    }
    int16_t accelerometerY() {
        return sb10Frame.accelerometer[0];
    }
    int16_t accelerometerZ() {
        return sb10Frame.accelerometer[2];
    }
    uint16_t light() {
        return sb10Frame.light;
    }
    uint16_t analogInput(int index) {
        if (index < 0 || index > 4)
            return 0;
        return sb10Frame.analogInputs[index];
    }
    unsigned long dataAgeMillis() {
        return millis() - lastFrameTime;
    }

    bool readNextFrame() {
        if (source.available() < 1)
            return false;
        unsigned long timeout = 15;
        unsigned long startMs = millis();
        do {
            int c = source.read();
            if (c < 0)
                continue;
            if (processFrameByte(c))
                return true;
        } while (millis() - startMs < timeout);
        //in event of timeout reset state to sync back to header
        mode = HEADER;
        pos = 0;
        return false;
    }

    void ICACHE_RAM_ATTR readAvailable() {
        int c;
        while ((c = source.read()) >= 0) {
            processFrameByte(c);
        }
    }

    bool ICACHE_RAM_ATTR processFrameByte(uint8_t c) {
        bool result = false;
        switch (mode) {
            case HEADER:
                if (c != "SB1.0"[pos++]) //sync to header
                    pos = 0;
                if (pos == 6) { //full header found, including null byte
                    mode = DATA;
                    pos = 0;
                }
                break;
            case DATA:
                frameBuffer[pos++] = c;
                if (pos == sizeof(SB10Frame)) {
                    mode = END;
                    pos = 0;
                }
                break;
            case END:
                if (c != "END"[pos++]) {//sync to header
                    //lost sync somewhere in the middle, throw away the frame
                    mode = HEADER;
                    pos = 0;
                }
                if (pos == 4) { //full footer found, including null byte
                    mode = HEADER;
                    pos = 0;
                    memcpy(&sb10Frame, frameBuffer, sizeof(SB10Frame));
                    lastFrameTime = millis();
                    result = true;
                }
        }
        return result;
    }
};

#endif //SB_ARDUINO_PIXELBLAZESENSORBOARD_H
