
#pragma once

#include <AServoHal.h>

class Servo;
class DbgTrace_Port;

/**
 * @brief 
 * 
 */
class MyServoHal : public AServoHal
{
public:
  MyServoHal(int servoPin);
  virtual ~MyServoHal();

  void setAngle(int angle);

public:
  static const int c_maxAngleLimit;  /// [°]
  static const int c_minAngleLimit;  /// [°]

private:
  Servo* m_servo;
  DbgTrace_Port* m_trPort;
};
