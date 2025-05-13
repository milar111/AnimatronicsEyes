#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_MIN 102   
#define SERVO_MAX 512    
#define SERVO_CH   2    

void setServoAngle(uint8_t ch, int angle) {
  angle = constrain(angle, 0, 180);
  uint16_t pulse = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
  pwm.setPWM(ch, 0, pulse);
  Serial.print("Servo "); Serial.print(ch);
  Serial.print(" -> "); Serial.print(angle); Serial.println("°");
}

void setup() {
  Serial.begin(115200);
  Wire.begin();            
  pwm.begin();
  pwm.setPWMFreq(50);        
  delay(200);

  setServoAngle(SERVO_CH, 90);
  Serial.println("Enter angle (0–180) and press ENTER:");
}

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.length()) {
      int angle = line.toInt();
      if (angle < 0 || angle > 180) {
        Serial.println("Error: angle must be 0–180");
      } else {
        setServoAngle(SERVO_CH, angle);
      }
    }
  }
}
