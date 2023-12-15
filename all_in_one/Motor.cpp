#include "Motor.h"

Motor::Motor(int motor_pin){
  attachPin(motor_pin);
}

Motor::Motor(){};

void Motor::attachPin(int motor_pin){
  _motor_pin = motor_pin;
  motor_positive_modifer = 2.0;
  motor_negative_modifer = 2.0;
  motor_deadzone_negative = 50;
  motor_deadzone_positive = 50;
}

void Motor::reset(){
  _servo.attach(_motor_pin);
}

int Motor::goms(int ms){
  int modifier = 1;
  if (ms>MIDPOINT) modifier = motor_positive_modifer;
  if (ms<MIDPOINT) modifier = motor_negative_modifer;
  int delta = ms-MIDPOINT;
  //map around deadzone
  int predeadzonems = constrain(MIDPOINT+delta*modifier,1100,1900);
  int finalms = MIDPOINT;
  if (predeadzonems < MIDPOINT){
     finalms = map(predeadzonems, 1100, MIDPOINT, 1100, MIDPOINT-motor_deadzone_negative);
  } else if (predeadzonems > MIDPOINT) {
     finalms = map(predeadzonems, MIDPOINT, 1900, MIDPOINT+motor_deadzone_positive, 1900);
  }

  _servo.writeMicroseconds(constrain(finalms,1100,1900));
  return _servo.readMicroseconds();
}

int Motor::goms_mnp(int ms){
  int modifier = 1;
  if (ms>MIDPOINT_MNP) modifier = motor_positive_modifer;
  if (ms<MIDPOINT_MNP) modifier = motor_negative_modifer;
  int delta = ms-MIDPOINT_MNP;
  //map around deadzone
  int predeadzonems = constrain(MIDPOINT_MNP+delta*modifier,1000,2000);
  int finalms = MIDPOINT_MNP;
  if (predeadzonems < MIDPOINT_MNP){
     finalms = map(predeadzonems, 1000, MIDPOINT_MNP, 1000, MIDPOINT_MNP-motor_deadzone_negative);
  } else if (predeadzonems > MIDPOINT_MNP) {
     finalms = map(predeadzonems, MIDPOINT_MNP, 2000, MIDPOINT_MNP+motor_deadzone_positive, 2000);
  }

  _servo.writeMicroseconds(constrain(finalms,1000,2000));
  return _servo.readMicroseconds();
}

void Motor::stop(){
  _servo.writeMicroseconds(MIDPOINT);
  _servo.detach();
}

bool Motor::attached(){
  return _servo.attached();
}
