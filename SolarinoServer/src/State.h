#ifndef State_h
#define State_h

#include <Arduino.h>

#include "TimeClient.h"

struct Statistics
{
    bool coolerOn;
    bool sourceOn;
    bool emergency;
    bool emergencyDisabled;
    bool systemOK;
    float ampers;
    float systemVoltage;
    String time;
};

class State
{
private:
    uint16_t emergencyCounter = 0;
    uint16_t frezerCounter = 0;

    float _amps = 0;
    bool _coolerOn = false;
    bool _sourceOn = false;
    bool _emergency = false;
    bool _emergencyDisabled = false;
    bool _systemOK = false;
    float _systemVoltage = 0;

public:
    void incEmergencyCounter()
    {
        emergencyCounter++;
    }

    void incfrezerCounter()
    {
        emergencyCounter++;
    }

    void setAmps(float amps)
    {
        _amps = amps;
    }

    void setSystemVoltage(float volt)
    {
        _systemVoltage = volt;
    }

    void setCoolerOn(bool on)
    {
        _coolerOn = on;
    }

    void setSourceOn(bool on)
    {
        _sourceOn = on;
    }

    void setEmergencyOn(bool on)
    {
        _emergency = on;
    }

    void setEmergencyDesabled(bool disabled)
    {
        _emergencyDisabled = disabled;
    }

    void setSystemOK(bool ok)
    {
        _systemOK = ok;
    }

    Statistics getStats()
    {
        _sourceOn = !_sourceOn;
        _systemVoltage = 12.0 + (rand() % 100)/100.0;
        Statistics s;
        s.ampers = _amps;
        s.coolerOn = _coolerOn;
        s.emergency = _emergency;
        s.emergencyDisabled = _emergencyDisabled;
        s.sourceOn = _sourceOn;
        s.systemOK = _systemOK;
        s.systemVoltage = _systemVoltage;
        s.time = ntpAsctime();
        return s;
    }

} state;

#endif