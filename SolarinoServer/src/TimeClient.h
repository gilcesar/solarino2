#ifndef TimeClient_h
#define TimeClient_h

#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.nist.gov", -3 * 3600, 5 * 1000);

void setupTime()
{
  timeClient.begin();
}

bool isDateTimeUpdated(tm *tm)
{
  return tm->tm_year >= 2000;
}

void updateTime(){
    time_t tt = timeClient.getEpochTime();
    tm *tm = localtime(&tt);
    if (isDateTimeUpdated(tm))
    {
      timeClient.setUpdateInterval(60 * 60 * 1000);//1h
    }
    timeClient.update();
    printf("The local date and time is: %s", asctime(tm));
}

String ntpAsctime(){
    time_t tt = timeClient.getEpochTime();
    tm *tm = localtime(&tt);
    return String(asctime(tm));
}


#endif