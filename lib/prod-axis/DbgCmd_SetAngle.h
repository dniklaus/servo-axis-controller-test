/*
 * DbgCmd_SetAngle.h
 *
 *  Created on: 18.08.2022
 *      Author: nid
 */

#ifndef SRC_DbgCmd_SetAngle_H_
#define SRC_DbgCmd_SetAngle_H_

#include <DbgCliCommand.h>

class Axis;
class DbgTrace_Port;

class DbgCmd_SetAngle : public DbgCli_Command
{
public:
  DbgCmd_SetAngle(Axis* axis);
  virtual ~DbgCmd_SetAngle();

  Axis* axis();

  void execute(unsigned int argc, const char** args, unsigned int idxToFirstArgToHandle);
  void printUsage();

private:
  Axis* m_axis;
  DbgTrace_Port* m_trPort;

private:  // forbidden default functions
  DbgCmd_SetAngle();                                        // default constructor
  DbgCmd_SetAngle& operator = (const DbgCmd_SetAngle& src); // assignment operator
  DbgCmd_SetAngle(const DbgCmd_SetAngle& src);              // copy constructor
};

#endif // SRC_DbgCmd_SetAngle_H_
