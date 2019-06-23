#ifndef Gip_h
#define Gip_h

#include <Arduino.h>
#include <Wire.h>

struct GMessage
{
    uint16 id;
    uint16 size;
    byte *data;
};

class Gip
{
private:
public:
    void init()
    {
        Wire.begin(); // join i2c bus (address optional for master)

        //Wire.begin(8);                // join i2c bus with address #8
        //Wire.onReceive(this->receiveEvent); // register event
    }

    void receiveEvent(int howMany)
    {
        while (1 < Wire.available())
        {                         // loop through all but the last
            char c = Wire.read(); // receive byte as a character
            Serial.print(c);      // print the character
        }
        int x = Wire.read(); // receive byte as an integer
        Serial.println(x);   // print the integer
    }

    GMessage sendMessage(uint16 id, uint16 size, byte *data)
    {
        GMessage msg;
        msg.id = id;
        msg.size = size;
        msg.data = data;

        Wire.beginTransmission(8); // transmit to device #8
        Wire.write((const char *)&msg);
        Wire.endTransmission(); // stop transmitting
        return msg;
    }

    void onMessage(void (*onMessageFunPtr)(GMessage message))
    {
        GMessage m;
        onMessageFunPtr(m);
    }
};

#endif