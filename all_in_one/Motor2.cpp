#include "Motor2.h"

Motor2::Motor2(int motor2_pin){
  attachPin(motor2_pin);
}

Motor2::Motor2(){};

void Motor2::attachPin(int motor2_pin){
  _motor2_pin = motor2_pin;
  motor2_positive_modifer = 2.0;
  motor2_negative_modifer = 2.0;
  motor2_deadzone_negative = 50;
  motor2_deadzone_positive = 50;
}

void Motor2::reset(){
  _servo2.attach(_motor2_pin);
}

int Motor2::goms2(int ms){
  int modifier = 1;
  if (ms>MIDPOINT2) modifier = motor2_positive_modifer;
  if (ms<MIDPOINT2) modifier = motor2_negative_modifer;
  int delta = ms-MIDPOINT2;
  //map around deadzone
  int predeadzonems = constrain(MIDPOINT2+delta*modifier,1100,1900);
  int finalms = MIDPOINT2;
  if (predeadzonems < MIDPOINT2){
     finalms = map(predeadzonems, 1100, MIDPOINT2, 1100, MIDPOINT2-motor2_deadzone_negative);
  } else if (predeadzonems > MIDPOINT2) {
     finalms = map(predeadzonems, MIDPOINT2, 1900, MIDPOINT2+motor2_deadzone_positive, 1900);
  }

  _servo2.writeMicroseconds(constrain(finalms,1100,1900));
  return _servo2.readMicroseconds();
}

int Motor2::goms2_mnp(int ms){
  int modifier = 1;
  if (ms>MIDPOINT2_MNP) modifier = motor2_positive_modifer;
  if (ms<MIDPOINT2_MNP) modifier = motor2_negative_modifer;
  int delta = ms-MIDPOINT2_MNP;
  //map around deadzone
  int predeadzonems = constrain(MIDPOINT2_MNP+delta*modifier,1000,2000);
  int finalms = MIDPOINT2_MNP;
  if (predeadzonems < MIDPOINT2_MNP){
     finalms = map(predeadzonems, 1000, MIDPOINT2_MNP, 1000, MIDPOINT2_MNP-motor2_deadzone_negative);
  } else if (predeadzonems > MIDPOINT2_MNP) {
     finalms = map(predeadzonems, MIDPOINT2_MNP, 2000, MIDPOINT2_MNP+motor2_deadzone_positive, 2000);
  }

  _servo2.writeMicroseconds(constrain(finalms,1000,2000));
  return _servo2.readMicroseconds();
}

void Motor2::stop(){
  _servo2.writeMicroseconds(MIDPOINT2);
  _servo2.detach();
}

bool Motor2::attached(){
  return _servo2.attached();
}
