
#include <FS.h>
#include <Thread.h>
#include <ThreadController.h>
#include <ThreadRunOnce.h>

#include "Gip.h"
#include "WebServer.h"
#include "TimeClient.h"

#define DBG_OUTPUT_PORT Serial

typedef void(ThreadCallback)();
ThreadController threadCtrl = ThreadController();

void timerCallback()
{
  threadCtrl.run();
}

Thread *addThread(Thread *thread, ThreadCallback *callback, int interval)
{
  thread->onRun(callback);
  thread->setInterval(interval);
  threadCtrl.add(thread);
  return thread;
}

Thread *createAndAddThread(ThreadCallback *callback, int interval)
{
  return addThread(new Thread(), callback, interval);
}

ThreadRunOnce *createAndAddThreadRunOnce(ThreadCallback *callback)
{
  ThreadRunOnce *t = new ThreadRunOnce();
  t->onRun(callback);
  threadCtrl.add(t);
  return t;
}

void showTime()
{
  //DBG_OUTPUT_PORT.println(timeClient.getFormattedTime());
}

void setupLog()
{
  DBG_OUTPUT_PORT.begin(115200);
  DBG_OUTPUT_PORT.print("\n");
  DBG_OUTPUT_PORT.setDebugOutput(true);
}

void setupFileSystem()
{
  SPIFFS.begin();
}

void setup(void)
{
  setupLog();
  setupFileSystem();

  createAndAddThread([]() { webserver.runWebServer(); }, 10);

  createAndAddThread([]() { timeClient.updateTime(); }, 5 * 1000);

}

void loop(void)
{
  delay(10);
  timerCallback();
}