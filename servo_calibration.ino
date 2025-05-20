#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_MIN 102
#define SERVO_MAX 512

#define SERVO_CENTER map(90, 0, 180, SERVO_MIN, SERVO_MAX)

#define SERVO_1 0
#define SERVO_2 1
#define SERVO_3 2
#define SERVO_4 3

void setup() {
  Serial.begin(115200);
  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(50);  

  delay(500);
  uint16_t pulse = SERVO_CENTER;
  Serial.print("Centering servos to pulse: "); Serial.println(pulse);

  pwm.setPWM(SERVO_1, 0, pulse);
  pwm.setPWM(SERVO_2, 0, pulse);
  pwm.setPWM(SERVO_3, 0, pulse);
  pwm.setPWM(SERVO_4, 0, pulse);
}

void loop() {
}
SSS