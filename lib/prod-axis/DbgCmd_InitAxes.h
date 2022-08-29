/*
 * DbgCmd_InitAxes.h
 *
 *  Created on: 18.08.2022
 *      Author: nid
 */

#ifndef SRC_DbgCmd_InitAxes_H_
#define SRC_DbgCmd_InitAxes_H_

#include <DbgCliCommand.h>

class CmdSequence;
class DbgTrace_Port;

class DbgCmd_InitAxes : public DbgCli_Command
{
public:
  DbgCmd_InitAxes(CmdSequence* axesInitSequence);
  virtual ~DbgCmd_InitAxes();

  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle);
  void printUsage();

private:
  CmdSequence* m_axesInitSequence;
  DbgTrace_Port* m_trPort;

private:  // forbidden default functions
  DbgCmd_InitAxes();                                        // default constructor
  DbgCmd_InitAxes& operator = (const DbgCmd_InitAxes& src); // assignment operator
  DbgCmd_InitAxes(const DbgCmd_InitAxes& src);              // copy constructor
};

#endif // SRC_DbgCmd_InitAxes_H_
