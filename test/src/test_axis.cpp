/*
 * test_fishCollection.cpp
 *
 *  Created on: 07.02.2018
 *      Author: niklausd
 */

#include <gtest/gtest.h>

#include <Axis.h>

class TestServo : public IServoHal
{
private:
  int m_angle;
public:
  TestServo()
  : m_angle(0)
  { }

  void setAngle(int angle)
  {
    m_angle = angle;
  }

  int getAngle()
  {
    return m_angle;
  }
};

TEST(axis, attachTestServo)
{
  TestServo* testServo = new TestServo();
  Axis* testAxis = new Axis();
  testAxis->attachServoHal(testServo);

  ASSERT_FALSE(testServo == 0);
  ASSERT_FALSE(testAxis == 0);

  ASSERT_TRUE(testServo->getAngle() == 0);
}
