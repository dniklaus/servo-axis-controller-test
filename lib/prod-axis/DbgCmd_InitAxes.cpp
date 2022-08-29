#include <CmdSequence.h>
#include <DbgTracePort.h>
#include <DbgCliTopic.h>
#include <DbgCliNode.h>
#include "DbgCmd_InitAxes.h"

DbgCmd_InitAxes::DbgCmd_InitAxes(CmdSequence* axesInitSequence)
: DbgCli_Command(DbgCli_Node::RootNode(), "init", "Initialize all axes.")
, m_axesInitSequence(axesInitSequence)
, m_trPort(new DbgTrace_Port("init", DbgTrace_Level::info))
{ }

DbgCmd_InitAxes::~DbgCmd_InitAxes() 
{ }

void DbgCmd_InitAxes::execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle)
{
  if (argc - idxToFirstArgToHandle != 0)
  {
    printUsage();
  }
  else
  {
    m_axesInitSequence->start();
    TR_PRINTF(m_trPort, DbgTrace_Level::info, "Initialize all axes.");
  }
}

void DbgCmd_InitAxes::printUsage()
{
  Serial.println(getHelpText());
  Serial.println("Usage: dbg init");
}
