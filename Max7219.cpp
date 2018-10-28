#include "Max7219.h"

Max7219::Max7219(unsigned char dataPin, unsigned char clockPin,  unsigned char loadPin)
{
    if(wiringPiSetup() == -1)
    {
        printf("wiringPiSetup() failed!\n");
        exit(1);
    }

    // We need 3 output pins to control the Max7219: Data, Clock and Load
    this->dataPin = dataPin;
    this->clockPin = clockPin;
    this->loadPin = loadPin;
    pinMode(dataPin, OUTPUT);  
    pinMode(clockPin, OUTPUT);
    pinMode(loadPin, OUTPUT);  

    // Set defaults.
    Send(SCAN_LIMIT, 7);    // set up to scan all eight digits
    Send(DECODE_MODE, 0);   // Set BCD decode mode off
    Send(DISPLAY_TEST, 0);  // Disable test mode
    Send(INTENSITY, 1);     // set brightness 0 to 15
    Send(SHUTDOWN, 1);      // come out of shutdown mode	/ turn on the digits
    
    // Clear display.
    this->Clear();

    printf ("Raspberry Pi Max7219 Matrix Display using WiringPi initialized!\n");
}

// Take a reg numer and data and send to the max7219.
void Max7219::Send (unsigned char reg_number, unsigned char dataout)
{
    digitalWrite(this->loadPin, 1);  // set LOAD 1 to start
    Send16bits((reg_number << 8) + dataout);   // send 16 bits ( reg number + dataout )
    digitalWrite(this->loadPin, 0);  // LOAD 0 to latch
    digitalWrite(this->loadPin, 1);  // set LOAD 1 to finish
}

// Clear matrix display by setting all lines to 0.
void Max7219::Clear()
{
    for (int i=1; i<=8; i++)
    {
        this->Send(i, 0);
    }
}

// Set brightness 0 to 15
void Max7219::SetBrightness(unsigned char brightness)
{
    this->Send(INTENSITY, brightness);
}

// Helper function.
void Max7219::Send16bits (unsigned short output)
{
    unsigned char i;
    for (i=16; i>0; i--) 
    {
        unsigned short mask = 1 << (i - 1); // calculate bitmask
        digitalWrite(this->clockPin, 0);  // set clock to 0

        // Send one bit on the data pin
        if (output & mask) 
            digitalWrite(this->dataPin, 1);
        else 
            digitalWrite(this->dataPin, 0);
                    
        digitalWrite(this->clockPin, 1);  // set clock to 1
    }
}