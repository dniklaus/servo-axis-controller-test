/*
 * Axis.cpp
 *
 *  Created on: 17.01.2018
 *      Author: nid
 */

#include <Timer.h>

#include <Axis.h>

class VelocityControlTimerAdapter : public TimerAdapter
{
private:
  Axis* m_axis;

public:
  VelocityControlTimerAdapter(Axis* axis)
  : m_axis(axis)
  { }

  void timeExpired()
  {
    if (0 != m_axis)
    {
      m_axis->doAngleControl();
    }
  }
};

//-----------------------------------------------------------------------------

Axis::Axis()
: m_servoHal(0)
, m_angleMin(-90)
, m_angleMax(90)
, m_angle(0)
, m_velocity(0)
, m_targetAngle(0)
, m_velocityControlTimer(new Timer(new VelocityControlTimerAdapter(this), Timer::IS_RECURRING, 10))
, m_targetReachedNotifier(0)
{ }

Axis::~Axis()
{
  delete m_velocityControlTimer->adapter();
  m_velocityControlTimer->attachAdapter(0);

  delete m_velocityControlTimer;
  m_velocityControlTimer = 0;
}

