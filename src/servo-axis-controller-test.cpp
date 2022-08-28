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
#include <Indicator.h>
#include <MyBuiltinLedIndicatorAdapter.h>
#include <ITargetReachedNotifier.h>
#include <TargetReachedNotifier.h>
#include <ProductDebug.h>
#include <CmdSequence.h>
#include <Cmd.h>
#include <Axis.h>
#include <MyServoHal.h>
#include <DbgCmd_SetAngle.h>

// Heart beat indicator implementation with built in LED
Indicator* heartbeat  = 0;

SerialCommand* sCmd = 0;
Axis* axis = 0;
int servoPin = 2;

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
  setupProdDebugEnv();

  heartbeat = new Indicator("heartbeat", "Hear Beat indicator LED.");
  heartbeat->assignAdapter(new MyBuiltinLedIndicatorAdapter());

  char* axisName;
  for (unsigned int i = 0; i < 5; i++)
  {
    axisName = new char[6];
    memset(axisName, 0, strlen(axisName));
    sprintf(axisName, "ax%d\0", i);
    axis = new Axis(axisName);
    axis->attachServoHal(new MyServoHal(servoPin+i));
    new DbgCmd_SetAngle(axis);
    
    CmdSequence* cmdSequence = new CmdSequence();
    axis->attachTargetReachedNotifier(new TargetReachedNotifier(axis, cmdSequence));

    new CmdGoToAngle(cmdSequence, -1, axis, 40, 50);
    new CmdStop(cmdSequence, 100);
    new CmdGoToAngle(cmdSequence, -1, axis, -40, 50);
    new CmdStop(cmdSequence, 100);
    new CmdGoToAngle(cmdSequence, -1, axis, 40, 50);
    new CmdStop(cmdSequence, 100);
    new CmdGoToAngle(cmdSequence, -1, axis, 0, 50);

    cmdSequence->start();
    delayAndSchedule(1500);
  }
     
  Axis* ax0 = static_cast<DbgCmd_SetAngle*>(DbgCli_Node::RootNode()->getChildNode("ax0")->getChildNode("set"))->axis();
  Axis* ax1 = static_cast<DbgCmd_SetAngle*>(DbgCli_Node::RootNode()->getChildNode("ax1")->getChildNode("set"))->axis();
  Axis* ax2 = static_cast<DbgCmd_SetAngle*>(DbgCli_Node::RootNode()->getChildNode("ax2")->getChildNode("set"))->axis();
  Axis* ax3 = static_cast<DbgCmd_SetAngle*>(DbgCli_Node::RootNode()->getChildNode("ax3")->getChildNode("set"))->axis();
  Axis* ax4 = static_cast<DbgCmd_SetAngle*>(DbgCli_Node::RootNode()->getChildNode("ax4")->getChildNode("set"))->axis();

  if ((0 != ax0) && (0 != ax1) && (0 != ax2) && (0 != ax3) && (0 != ax4))
  {
    while (ax0->isBusy() || 
           ax1->isBusy() ||
           ax2->isBusy() ||
           ax3->isBusy() ||
           ax4->isBusy()  ) 
    { 
      scheduleTimers(); 
    }
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
