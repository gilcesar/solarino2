#ifndef VoltageSensor_h
#define VoltageSensor_h

#include <Arduino.h>
#include "Sensor.h"

const int SAMPLES = 20;

class VoltageSensor : public Sensor
{
private:
  Scale scale = Scale(0, 1023, 0, 25);
  int values[SAMPLES];
  //uint64_t time = millis();
  int pos = 0;
  float gain = 1.0;
  int media()
  {
    float sum = 0;
    for (int i = 0; i < SAMPLES; i++)
    {
      sum += values[i];
    }
    return sum / SAMPLES;
  }

  void resetValues()
  {
    for (int i = 0; i < SAMPLES; i++)
    {
      values[i] = 0;
    }
  }

public:
  VoltageSensor(int pin, float gain) : Sensor(pin)
  {
    resetValues();
    this->gain = gain;
  }

  float getValue()
  {
    return scale.getScaled(media()) * gain;
  }

  void setGain(float gain)
  {
    this->gain = gain;
  }

  void update()
  {
    values[pos++] = getRawValue();
    if (pos == SAMPLES)
    {
      pos = 0;
    }
  }
};

#endif