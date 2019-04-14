#include <common.h>
#include <stdarg.h>
#include <stdio.h>
#include "user_config.h"


//Globals
bool globalIsWifiConnected  = false;

String log_buffer[LOG_SIZE];
unsigned long log_buffer_ms[LOG_SIZE];
int log_buffer_index = -1;

void setupCommon()
{
  for (int i = 0; i < LOG_SIZE; i++)
  {
    log_buffer[i] = "";
    log_buffer_ms[i] = 0;
  }
  if (LOG_TO_SERIAL)
  {
    Serial.begin(115200);
  }
}

void writeToLog(String line, ...)
{
  char temp[300];
  const char *l = line.c_str();
  log_buffer_index++;
  if (log_buffer_index == LOG_SIZE)
  {
    log_buffer_index = 0;
  }
  va_list args;
  va_start(args, l);
  vsprintf(temp, l, args);
  log_buffer[log_buffer_index] = temp;
  log_buffer_ms[log_buffer_index] = millis();
  va_end(args);
  if (LOG_TO_SERIAL)
  {
    Serial.println(temp);
  }
}

String readLogBuffer()
{
  String output;
  unsigned long now = millis();
  for (int i = LOG_SIZE; i > 0; i--)
  {
    int p = i + log_buffer_index;
    int l = p % LOG_SIZE;
    unsigned long temp = (now - log_buffer_ms[l]) / 1000;
    if (temp < 10)
    {
      output += "0";
    }
    output += temp;
    output += " - ";
    output += log_buffer[l];
    output += "\n";
  }
  return output;
}
