#ifndef _COMMON_H_
#define _COMMON_H_
#include <Arduino.h>


//Globals
extern bool globalIsWifiConnected;


//OTA
#define FW_VERSION 1


void setupCommon();
void writeToLog(String line, ...);
String readLogBuffer();

#endif