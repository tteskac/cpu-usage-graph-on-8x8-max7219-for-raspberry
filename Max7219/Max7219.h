#pragma once

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Example pin wiring for matrix display on Raspberry Pi:
// DATA/DIN:    0 - GPIO 17 (WiringPi pin num 0) header pin 11
// CLOCK/CLK:   3 - GPIO 22 (WiringPi pin num 3) header pin 15
// LOAD/CS:     4 - GPIO 23 (WiringPi pin num 4) header pin 16
// Note: no SPI/I2C pins needed, choose any available digital GPIO.

// The Max7219 Registers :
#define DECODE_MODE   0x09                       
#define INTENSITY     0x0a                        
#define SCAN_LIMIT    0x0b                        
#define SHUTDOWN      0x0c                        
#define DISPLAY_TEST  0x0f    

class Max7219
{
    public:
        Max7219(unsigned char dataPin, unsigned char clockPin,  unsigned char loadPin);
        void Send(unsigned char reg_number, unsigned char dataout);
        void Clear();
        void SetBrightness(unsigned char brightness);
    private:
        void Send16bits(unsigned short output);
    
    private:
        unsigned char dataPin, clockPin, loadPin;

};
