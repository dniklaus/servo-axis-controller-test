/*
 * ProductDebug.cpp
 *
 *  Created on: 14.06.2016
 *      Author: nid
 */

#include "ProductDebug.h"

#include <Arduino.h>
#include <AppDebug.h>


#ifdef ESP8266
extern "C"
{
  #include "user_interface.h"
}
#else
#include <RamUtils.h>
#endif

//-----------------------------------------------------------------------------

void setupProdDebugEnv()
{
  setupDebugEnv('\0');

  Serial.println();
  Serial.println("------------------------------------------");
  Serial.println("Hello from Servo Axis Controller Test App!");
  Serial.println("------------------------------------------");
  Serial.println();
}

