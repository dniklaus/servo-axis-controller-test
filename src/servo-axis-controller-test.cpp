/*
 * wiring-skeleton.cpp
 *
 *  Created on: 15.03.2017
 *      Author: niklausd
 */

#include <Arduino.h>

// PlatformIO libraries
#include <SerialCommand.h>  // pio lib install 173, lib details see https://github.com/kroimon/Arduino-SerialCommand
#include <Timer.h>          // pio lib install 1699, lib details see https://github.com/dniklaus/wiring-timer

// private libraries
#include <DbgCliNode.h>
#include <DbgCliTopic.h>
#include <DbgCliCommand.h>
#include <DbgTracePort.h>
#include <DbgTraceContext.h>
#include <DbgTraceOut.h>
#include <DbgPrintConsole.h>
#include <DbgTraceLevel.h>
#include <AppDebug.h>
#include <ProductDebug.h>
#include <RamUtils.h>
#include <Axis.h>
#include <Servo.h>

#ifndef BUILTIN_LED
#define BUILTIN_LED 13
#endif

SerialCommand* sCmd = 0;
Axis* axis = 0;
int servoPin = 6;

class ServoAxis : public IServoHal
{
  Servo* m_servo;
public:
  ServoAxis(int servoPin)
  : m_servo(new Servo())
  {
    m_servo->attach(servoPin);
  }
  void setAngle(int angle)
  {
    m_servo->write(angle);
  }
};

class DbgCmd_SetAngle : public DbgCli_Command
{
  Axis* m_axis;
public:
  DbgCmd_SetAngle(Axis* axis)
  : DbgCli_Command(new DbgCli_Topic(DbgCli_Node::RootNode(), "axis", "Axis debug commands"), "set", "Set angle [-90°..90°].")
  , m_axis(axis)
  { }

  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
  {
    int angle = 0;
    int velocity = 0;
    if (argc - idxToFirstArgToHandle != 2)
    {
      printUsage();
    }
    else
    {
      angle = atoi(args[idxToFirstArgToHandle]);
      velocity = atoi(args[idxToFirstArgToHandle+1]);
      m_axis->goToTargetAngle(angle, velocity);
      Serial.println();
      Serial.print("Axis set angle: ");
      Serial.print(angle);
      Serial.print(" [°]");
      Serial.print(", velocity: ");
      Serial.print(velocity);
      Serial.println(" [°/ms]");
    }
  }

  void printUsage()
  {
    Serial.println(getHelpText());
    Serial.println("Usage: dbg axis set <angle> <velocity>");
    Serial.println("       angle: -90..90, velocity: 1..40");
  }
};

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 0);

  setupProdDebugEnv();

  axis = new Axis();
  axis->attachServoHal(new ServoAxis(servoPin));

  new DbgCmd_SetAngle(axis);
}

void loop()
{
  if (0 != sCmd)
  {
    sCmd->readSerial();     // process serial commands
  }
  yield();                  // process Timers
}
