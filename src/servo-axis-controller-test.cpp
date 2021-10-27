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
#include <DbgCliNode.h>

// private libraries
#include <ITargetReachedNotifier.h>
#include <TargetReachedNotifier.h>
#include <ProductDebug.h>
#include <CmdSequence.h>
#include <Cmd.h>
#include <Axis.h>
#include <MyServoHal.h>

#ifndef BUILTIN_LED
#define BUILTIN_LED 13
#endif

SerialCommand* sCmd = 0;
Axis* axis = 0;
int servoPin = 2;

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

  Axis* axis() { return m_axis; }

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
  , m_targetAngle(targetAngle)
  , m_velocity(velocity)
  { }

  virtual ~CmdGoToAngle() { }
  
  void execute() 
  {
    if (0 != m_axis)
    {
      m_axis->goToTargetAngle(m_targetAngle, m_velocity);
    }
  }

  void leave()
  {
    if (0 != axis)
    {
      m_axis->stop();
    }
  }
};

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 0);

  setupProdDebugEnv();

  char* axisName;
  for (unsigned int i = 0; i < 2; i++)
  {
    axisName = new char[6];
    memset(axisName, 0, strlen(axisName));
    sprintf(axisName, "ax%d\0", i);
    axis = new Axis(axisName);
    axis->attachServoHal(new MyServoHal(servoPin+i));
    new DbgCmd_SetAngle(axis);
    
    CmdSequence* cmdSequence = new CmdSequence();
    axis->attachTargetReachedNotifier(new TargetReachedNotifier(axis, cmdSequence));

    new CmdGoToAngle(cmdSequence, -1, axis, 90, 2);
    new CmdStop(cmdSequence, 2000);
    new CmdGoToAngle(cmdSequence, -1, axis, -90, 2);
    new CmdStop(cmdSequence, 2000);
    new CmdGoToAngle(cmdSequence, -1, axis, 0, 2);
    cmdSequence->start();
  }
   
  // Synchronized and sequential axes movements
  Axis* ax0 = static_cast<DbgCmd_SetAngle*>(DbgCli_Node::RootNode()->getChildNode("ax0")->getChildNode("set"))->axis();
  Axis* ax1 = static_cast<DbgCmd_SetAngle*>(DbgCli_Node::RootNode()->getChildNode("ax1")->getChildNode("set"))->axis();
  if ((0 != ax0) && (0 != ax1))
  {
    while (!ax0->isTargetReached() && !ax1->isTargetReached()) { scheduleTimers(); }
    ax0->goToTargetAngle(90, 10);
    while (!ax0->isTargetReached()) { scheduleTimers(); }
  
    ax1->goToTargetAngle(90, 10);
    while(!ax1->isTargetReached()) { scheduleTimers(); }
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
