/*
 * File:   ACS712.h
 * Author: Gilcesar Avila
 *
 * Created on April 9, 2018, 8:45 PM
 */

#ifndef ACS712Sensor_h
#define ACS712Sensor_h
#include "Arduino.h"
#include "Sensor.h"


//Acertar integer overflow 
enum MillisTime
{
    SECOND = 1000,
    MINUTE = 60 * SECOND,
    HOUR = 60 * MINUTE,
    DAY = 24 * HOUR
};

enum Model
{
    A30 = 66,
    A20 = 100,
    A05 = 185
};

enum Current
{
    AC,
    DC
};

class ACS712Sensor : public Sensor
{
  private:
    uint64_t INTERVAL = 50UL;
    Scale scale = Scale(511, 1023, 0, 2.5);

    int mVperAmp = Model::A30;
    Current currentType = Current::AC;
    int value;
    float voltage = 120;
    float i, iRms;
    float wattSec, wattMin, wattHour, wattDay;
    uint64_t wattTimeSec, wattTimeMin, wattTimeHour;
    uint64_t updateCounter = 0;
    float wattPeakDay = 0;
    float currentWatt = 0;
    uint64_t sampleTime;

    void calcIRms()
    {                             //AC
        iRms = calcI() / sqrt(2); // RMS
    }

    float calcI()
    { //DC
        float scaled = scale.getScaled(value);
        return i = scaled / (mVperAmp / 1000.0f);
    }

    void updateWatts()
    {
        currentWatt = getValue() * voltage;
        wattPeakDay = currentWatt > wattPeakDay ? currentWatt : wattPeakDay;
        wattSec += currentWatt;
        updateCounter++;
        if (millis() - wattTimeSec > SECOND)
        {
            wattMin += (wattSec / updateCounter);//avg of updates samples
            updateCounter = 0;
            wattSec = 0;
            wattTimeSec = millis();
        }

        if(millis() - wattTimeMin > MINUTE){
            wattHour += (wattMin / 60);//avg of seconds samples
            wattMin = 0;
            wattTimeMin = millis();
        }

        if(millis() - wattTimeHour > HOUR){
            wattDay += (wattHour / 24);//avg of seconds samples
            wattHour = 0;
            wattTimeHour = millis();
        }
    }

    //void reset

  public:
    ACS712Sensor(int pin, Current current) : Sensor(pin)
    {
        this->currentType = current;
        reset();
    }

    void reset()
    {
        value = getRawValue();
        wattSec = wattMin = wattHour = wattDay =
            wattPeakDay = currentWatt = i = iRms = 0.0f;

        wattPeakDay = currentWatt = updateCounter = 0;

        sampleTime = wattTimeSec = wattTimeMin = wattTimeHour = millis();
    }

    void setModel(Model model)
    {
        this->mVperAmp = model;
        reset();
    }

    float getValue()
    {
        return currentType == Current::AC ? iRms : i;
    }

    void update(float voltage)
    {
        int rawValue = getRawValue();
        this->voltage = voltage;
        
        if ((millis() - sampleTime) > INTERVAL)
        {
            calcIRms();
            value = rawValue;
            sampleTime = millis();
        }
        else
        {
            value = rawValue > value ? rawValue : value;
        }
    }

    float getWattCurrent()
    {
        return currentWatt;
    }

    float getWattPeakDay()
    {
        return wattPeakDay;
    }

    float getWattHora()
    {
        return wattHour / (millis() - wattTimeHour)/MINUTE;
    }

    float getWattDay()
    {
        return wattDay;
    }
};

#endif
