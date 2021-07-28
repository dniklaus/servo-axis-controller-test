/*
 * wiring-skeleton.cpp
 *
 *  Created on: 15.03.2017
 *      Author: niklausd
 */

#include <Arduino.h>

// PlatformIO libraries
#include <SerialCommand.h>  // pio lib install 173, lib details see https://github.com/kroimon/Arduino-SerialCommand
#include <SpinTimer.h>      // pio lib install 11599, lib details see https://github.com/dniklaus/spin-timer
#include <DbgTracePort.h>
#include <DbgCliCommand.h>
#include <DbgCliTopic.h>

// private libraries
#include <ProductDebug.h>
#include <CmdSequence.h>
#include <Cmd.h>
#include <Axis.h>
#include <Servo.h>

#ifndef BUILTIN_LED
#define BUILTIN_LED 13
#endif

SerialCommand* sCmd = 0;
Axis* axis = 0;
int servoPin = 2;

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
    m_servo->write(map(angle, -90, 90, 0, 180));
  }
};

class TargetReachedNotifier : public ITargetReachedNotifier
{
  Axis* m_axis;
  DbgTrace_Port* m_trPort;
  char* m_trPortName;
  bool m_isTargetReached;

public:
  TargetReachedNotifier(Axis* axis)
  : m_axis(axis)
  , m_trPort(0)
  , m_trPortName(new char[strlen(m_axis->name())+2])
  , m_isTargetReached(false)
  {
    memset(m_trPortName, 0, strlen(m_axis->name())+2);
    sprintf(m_trPortName, "t%s", m_axis->name());
    m_trPort = new DbgTrace_Port(m_trPortName, DbgTrace_Level::debug);
  }

  void notifyTargetReached(int targetAngle)
  {
    TR_PRINTF(m_trPort, DbgTrace_Level::debug, "Target reached (%d)", targetAngle);
    m_isTargetReached = true;
  }

  void waitForTargetReached()
  {
    while (!m_isTargetReached)
    {
      scheduleTimers();
    }
    m_isTargetReached = false;
  }
};

class DbgCmd_SetAngle : public DbgCli_Command
{
private:
  Axis* m_axis;
  DbgTrace_Port* m_trPort;
  
public:
  DbgCmd_SetAngle(Axis* axis)
  : DbgCli_Command(new DbgCli_Topic(DbgCli_Node::RootNode(), axis->name(), "Axis debug commands"), "set", "Set angle {-90..90} with defined velocity {1..40}.")
  , m_axis(axis)
  , m_trPort(new DbgTrace_Port(m_axis->name(), DbgTrace_Level::debug))
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
      TR_PRINTF(m_trPort, DbgTrace_Level::debug, "Moving from %d to %d, velocity: %d", m_axis->getAngle(), angle, velocity);
    }
  }

  void printUsage()
  {
    Serial.println(getHelpText());
    Serial.println("Usage: dbg axis set <angle> <velocity>");
    Serial.println("       angle: -90..90, velocity: 1..40");
  }
};

class CmdGoToAngle : public Cmd
{
private:
  Axis* m_axis;
  int m_targetAngle;
  int m_velocity;
public:
  CmdGoToAngle(CmdSequence* cmdSequence, long int timeMillis, Axis* axis, int targetAngle, int velocity) 
  : Cmd(cmdSequence, timeMillis, (0 != axis) ? axis->name() : "")
  , m_axis(axis)
  { }

  virtual ~CmdGoToAngle() { }
  
  void execute() 
  {
    if (0 != m_axis)
    {
      m_axis->goToTargetAngle(m_targetAngle, m_velocity);
    }
  }
};

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 0);

  setupProdDebugEnv();

  char* axisName;
  for (unsigned int i = 0; i < 1; i++)
  {
    axisName = new char[6];
    memset(axisName, 0, strlen(axisName));
    sprintf(axisName, "ax%d\0", i);
    axis = new Axis(axisName);
    axis->attachServoHal(new ServoAxis(servoPin+i));
    TargetReachedNotifier* targetReachedNotifier = new TargetReachedNotifier(axis);
    axis->attachTargetReachedNotifier(targetReachedNotifier);
    new DbgCmd_SetAngle(axis);
    CmdSequence* cmdSequence = new CmdSequence();
    new CmdGoToAngle(cmdSequence, 10000, axis, 30, 1);
    new CmdGoToAngle(cmdSequence, 10000, axis, -30, 1);
    new CmdGoToAngle(cmdSequence, 10000, axis, 0, 1);
  }
}

void loop()
{
  if (0 != sCmd)
  {
    sCmd->readSerial();     // process serial commands
  }
  scheduleTimers();         // process Timers
}
