#include <Axis.h>
#include <DbgTracePort.h>
#include <DbgCliTopic.h>
#include <DbgCliNode.h>
#include "DbgCmd_SetAngle.h"

DbgCmd_SetAngle::DbgCmd_SetAngle(Axis* axis)
: DbgCli_Command(new DbgCli_Topic(DbgCli_Node::RootNode(), axis->name(), "Axis debug commands"), "set", "Set angle [°] with defined velocity.")
, m_axis(axis)
, m_trPort(new DbgTrace_Port(m_axis->name(), DbgTrace_Level::info))
{ }

DbgCmd_SetAngle::~DbgCmd_SetAngle() 
{ }

Axis* DbgCmd_SetAngle::axis()
{ 
  return m_axis;
}

void DbgCmd_SetAngle::execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
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
        TR_PRINTF(m_trPort, DbgTrace_Level::info, "Moving from %d to %d, velocity: %d", m_axis->getAngle(), angle, velocity);
    }
}

void DbgCmd_SetAngle::printUsage()
{
    Serial.println(getHelpText());
    Serial.println("Usage: dbg axis set <angle> <velocity>");
    Serial.print  ("       angle: ");
    Serial.print  (m_axis->servoHal()->getMinAngleLimit());
    Serial.print  ("..");
    Serial.print  (m_axis->servoHal()->getMaxAngleLimit());
    Serial.println(", velocity: 1..500");
}
