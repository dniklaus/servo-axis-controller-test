
#pragma once

#include <Axis.h>

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

  // /**
  //  * @brief Get the Max Angle Limit object
  //  * 
  //  * @return int [°]
  //  */
  // int getMaxAngleLimit();

  // /**
  //  * @brief Get the Min Angle Limit object
  //  * 
  //  * @return int [°]
  //  */
  // int getMinAngleLimit();

// private:
  // static const int c_maxAngleLimit;  /// [°]
  // static const int c_minAngleLimit;  /// [°]

private:
  Servo* m_servo;
  DbgTrace_Port* m_trPort;
};
