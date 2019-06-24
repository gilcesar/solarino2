#ifndef TimeClient_h
#define TimeClient_h

#include <WiFiUdp.h>
#include <NTPClient.h>

class TimeClient
{
private:
  WiFiUDP ntpUDP;
  NTPClient *client;
public:
  TimeClient(const char* url, int timeOffset, int updateInterval){
    client = new NTPClient(ntpUDP, url, timeOffset, updateInterval);
    client->begin();
  }

  bool isDateTimeUpdated(tm *tm)
  {
    return tm->tm_year >= 2000;
  }

  void updateTime()
  {
    time_t tt = client->getEpochTime();
    tm *tm = localtime(&tt);
    if (!isDateTimeUpdated(tm))
    {
      client->forceUpdate();
      //client->setUpdateInterval(60 * 60 * 1000); //1h
    }
    client->update();
    printf("The local date and time is: %s", asctime(tm));
  }

  String formatedTime()
  {
    time_t tt = client->getEpochTime();
    tm *tm = localtime(&tt);
    return String(asctime(tm));
  }
} timeClient("time.nist.gov", -3 * 3600, 60*60*1000);

#endif