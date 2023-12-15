#ifndef __MOTOR2_H_
#define __MOTOR2_H_

#include "openrov_servo.h"
#include <Arduino.h>
#define MIDPOINT2 1499
#define MIDPOINT2_MNP 1499

class Motor2 {
  private:
    Servo _servo2;
    int _motor2_pin;

  public:
    Motor2(int motor2_pin);
    Motor2();
    void attachPin(int motor2_pin);
    int goms2(int ms);
    int goms2_mnp(int ms);
    void reset();
    void stop();
    bool attached();
    float motor2_positive_modifer;
    float motor2_negative_modifer;
    int motor2_deadzone_negative;
    int motor2_deadzone_positive;
};

#endif
