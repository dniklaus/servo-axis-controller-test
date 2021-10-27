#include <Axis.h>
#include <CmdSequence.h>
#include <DbgTraceLevel.h>
#include <DbgTracePort.h>
#include <TargetReachedNotifier.h>

TargetReachedNotifier::TargetReachedNotifier(Axis* axis, CmdSequence* cmdSequence)
: m_axis(axis)
, m_cmdSequence(cmdSequence)
, m_trPort(0)
, m_trPortName(new char[strlen(m_axis->name())+2])
{
  memset(m_trPortName, 0, strlen(m_axis->name())+2);
  sprintf(m_trPortName, "t%s", m_axis->name());
  m_trPort = new DbgTrace_Port(m_trPortName, DbgTrace_Level::debug);
}

TargetReachedNotifier::~TargetReachedNotifier()
{
  delete m_trPort;
  m_trPort = 0;

  delete [] m_trPortName;
  m_trPortName = 0;
}

void TargetReachedNotifier::notifyTargetReached(int targetAngle)
{
  TR_PRINTF(m_trPort, DbgTrace_Level::debug, "Target reached (%d)", targetAngle);
  if (0 != m_cmdSequence)
  {
    if (m_cmdSequence->isRunning())
    {
      m_cmdSequence->execNextCmd();
    }
  }
}
