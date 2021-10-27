
#pragma once

#include <Axis.h>

class Servo;

class MyServoHal : public IServoHal
{
public:
  MyServoHal(int servoPin);
  virtual ~MyServoHal();

  void setAngle(int angle);

private:
  Servo* m_servo;
};
