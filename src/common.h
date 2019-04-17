#ifndef _COMMON_H_
#define _COMMON_H_
#include <Arduino.h>


//Globals
extern bool globalIsWifiConnected;
extern bool globalIsLightsOn;
extern bool globalShouldPublishLongClick;


//OTA
#define FW_VERSION 1


void setupCommon();
void writeToLog(String line, ...);
String readLogBuffer();

typedef std::function<void(String)> LogLineHandlerFunction;

void onLogLine(LogLineHandlerFunction f);

#endif