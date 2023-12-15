#include <Wire.h>
#include "Timer.h"
#include "Command.h"
#include "Motor.h"
#include "Motor2.h"
#include "LiquidCrystal_I2C.h"
#include "MS5837.h"

LiquidCrystal_I2C lcd(0x3F, 16, 2);
Timer motor_time, check_time, kedalaman_time, lcd_timer, servo_time, kedalaman_timer;
Command cmd;
MS5837 sensor;
Motor motors[6] = {Motor(2), Motor(12), Motor(6), Motor(7),0,0};
Motor2 motors2[6] = {0,0,0,0,Motor2(4), Motor2(5)};
int da, y, depth_now;
int pwm_motor[6] = {1500, 1500, 1500, 1500, 1498, 1498};
int new_pwm[6] = {1500, 1500, 1500, 1500, 1498, 1498};

void setup()
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  Wire.begin();
  lcd.clear();
  motor_set_deadzone();
  motors2[4].goms2(1498);
  motors2[5].goms2(1498);
}

void loop()
{
  cmd.get();
  DoEverything(cmd);
  sensor_kedalaman();
  tulis();
  if (lcd_timer.elapsed(1000)) {
    lcd.clear();
  }
  kondisi_kedalaman();
  kontrol_motor();
}

void sensor_kedalaman() {
  while (!sensor.init()) {
    Serial.println("Init failed!");
  }
  if (kedalaman_timer.elapsed(100)) {
    sensor.read();
    depth_now = (sensor.depth()*1000+8600+952);
    Serial.print("$");
    Serial.println(depth_now);
  }
}

void motor_set_deadzone() {
  for (int i = 0; i < 6; i++) {
    if (i < 4){
      motors[i].motor_deadzone_negative = 50;
      motors[i].motor_deadzone_positive = 50;
      motors[i].reset();
      motors[i].goms(MIDPOINT);
    }
    else{
      motors2[i].motor2_deadzone_negative = 50;
      motors2[i].motor2_deadzone_positive = 50;
      motors2[i].reset();
      motors2[i].goms2(MIDPOINT2);
    }
  }
  delay(600);
}
  
void kontrol_motor() {
for (int i = 0; i < 6; i++) {
  if (pwm_motor[i] != new_pwm[i]) {
    pwm_motor[i] = new_pwm[i];
    if (i < 4) {
      motors[i].goms(new_pwm[i]);
    }else{
      motors2[i].goms2(new_pwm[i]);
    }
  }
}
}

void DoEverything(Command command) {
  if (command.cmp("go")) {
    for (int i = 2; i < 6; i++){
    new_pwm[i] = int(command.args[i-1]);
    }
  }
  if (command.cmp("motorv")) {
  for (int i = 0; i < 2; i++){
    new_pwm[i] = int(command.args[i+1]);
    }
  }
  if (command.cmp("stop")) {
    for (int i = 0; i < 6; i++){
    if (i < 4){
    new_pwm[i] = 1500;
    }else{
    new_pwm[i] = 1498;
    }}
  }
  if (command.cmp("da")) {
    da = bool(command.args[1]);
  }

  if (command.cmp("dt")) {
    y = int(command.args[1]);
    Serial.println("target_accepted");
  }
}

void tulis() {
  lcd.setCursor(9, 1);
  lcd.print(depth_now);
  lcd.setCursor(1, 0);
  lcd.print("N5-AUV");
  lcd.setCursor(1, 1);
}

void kondisi_kedalaman() {
  depth_now = (sensor.depth()*1000+8600+952);
  if (da == 1) {
    //Turun
    if (y > depth_now) {
      new_pwm[0] = new_pwm[1] = 1470;
    }
    //Naik
    else if (y < depth_now) {
    new_pwm[0] = new_pwm[1] = 1500;
    }
    //Di Lokasi
    else if ((depth_now - 1) <= y || y <= (depth_now + 1)) {
    new_pwm[0] = new_pwm[1] = 1500;
    }
  }
}
