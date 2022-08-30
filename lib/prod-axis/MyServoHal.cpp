
#include <MyServoHal.h>
#include <Arduino.h>
#include <Servo.h>
#include <DbgTraceContext.h>
#include <DbgTracePort.h>


const int MyServoHal::c_maxAngleLimit =  90;  /// [°]
const int MyServoHal::c_minAngleLimit = -90;  /// [°]

MyServoHal::MyServoHal(int servoPin)
: AServoHal(c_maxAngleLimit, c_minAngleLimit)
, m_servo(new Servo())
, m_trPort(0)
{
  m_servo->attach(servoPin);
}

MyServoHal::~MyServoHal() 
{ 
  delete m_servo;
  m_servo = 0;
}

void MyServoHal::setAngle(int angle)
{
  m_servo->write(map(angle, getMinAngleLimit(), getMaxAngleLimit(), 0, 180));

  Axis* myAxis = axis();
  if (0 != myAxis)
  {
    DbgTrace_Port* trPort = DbgTrace_Context::getContext()->getTracePort(myAxis->name());
    TR_PRINTF(trPort, DbgTrace_Level::debug, "Servo currently set angle to %d", myAxis->getAngle());
  }
}
