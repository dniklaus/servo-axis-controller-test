
#include <MyServoHal.h>
#include <Arduino.h>
#include <Servo.h>

MyServoHal::MyServoHal(int servoPin)
: m_servo(new Servo())
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
  m_servo->write(map(angle, -90, 90, 0, 180));
}
