
#pragma once

#include <Axis.h>

class Servo;
class DbgTrace_Port;

class MyServoHal : public AServoHal
{
public:
  MyServoHal(int servoPin);
  virtual ~MyServoHal();

  void setAngle(int angle);

private:
  Servo* m_servo;
  DbgTrace_Port* m_trPort;
};
