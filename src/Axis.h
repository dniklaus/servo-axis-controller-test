/*
 * Axis.h
 *
 *  Created on: 17.01.2018
 *      Author: nid
 */

#ifndef SRC_AXIS_H_
#define SRC_AXIS_H_

class IServoHal
{
public:
  virtual void setAngle(int angle) = 0;

public:
  virtual ~IServoHal() { }

protected:
  IServoHal() { }

private:  // forbidden functions
  IServoHal(const IServoHal& src);              // copy constructor
  IServoHal& operator = (const IServoHal& src); // assignment operator
};

class ITargetReachedNotifier
{
public:
  virtual void notifyTargetReached(int targetAngle) = 0;

public:
  virtual ~ITargetReachedNotifier() { }

private:  // forbidden functions
  ITargetReachedNotifier(const ITargetReachedNotifier& src);              // copy constructor
  ITargetReachedNotifier& operator = (const ITargetReachedNotifier& src); // assignment operator
};

class Timer;

class Axis
{
public:
  Axis();
  virtual ~Axis();

public:
  void attachServoHal(IServoHal* servoHal);

  void goToTargetAngle(int targetAngle, int velocity);
  void stop();

  void doAngleControl();

  int getAngle();

private:
  IServoHal* m_servoHal;
  int m_angleMin;
  int m_angleMax;
  int m_angle;
  int m_velocity;
  int m_targetAngle;
  unsigned long m_velocityCtrlIntervalMillis;
  static unsigned long s_defaultVelocityCtrlIntervalMillis;
  Timer* m_velocityControlTimer;
  ITargetReachedNotifier* m_targetReachedNotifier;

private:  // forbidden functions
  Axis(const Axis& src);              // copy constructor
  Axis& operator = (const Axis& src); // assignment operator
};

#endif /* SRC_AXIS_H_ */
