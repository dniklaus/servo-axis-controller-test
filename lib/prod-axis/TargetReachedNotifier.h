#pragma once

#include <ITargetReachedNotifier.h>

class Axis;
class CmdSequence;
class DbgTrace_Port;

class TargetReachedNotifier : public ITargetReachedNotifier
{
private:
  Axis* m_axis;
  CmdSequence* m_cmdSequence;
  DbgTrace_Port* m_trPort;
  char* m_trPortName;

public:
  TargetReachedNotifier(Axis* axis, CmdSequence* cmdSequence);
  virtual ~TargetReachedNotifier();

  virtual void notifyTargetReached(int targetAngle);

private:  // forbidden default functions
  TargetReachedNotifier();                                              // default constructor
  TargetReachedNotifier& operator = (const TargetReachedNotifier& src); // assignment operator
  TargetReachedNotifier(const TargetReachedNotifier& src);              // copy constructor
};
