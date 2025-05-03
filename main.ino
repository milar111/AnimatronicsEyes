#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Bluepad32.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); 

#define SERVO_CH1    0   // D-Pad Left/Right
#define SERVO_CH2    1   // D-Pad Up/Down

#define SERVO_MIN    102
#define SERVO_MAX    512

float currentAngle1 = 90, targetAngle1 = 90;
float currentAngle2 = 90, targetAngle2 = 90;

// Скоростни параметри
const int   maxSpeedDPS = 500;      
const int   loopDelayMs = 20;       
const float maxStep     = maxSpeedDPS * (loopDelayMs / 1000.0f);

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void onConnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++)
    if (!myControllers[i]) { myControllers[i] = ctl; break; }
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++)
    if (myControllers[i] == ctl) { myControllers[i] = nullptr; break; }
}

void processGamepad(ControllerPtr ctl) {
  uint8_t d = ctl->dpad();
  // Left/Right -> SERVO_CH1
  if (d & DPAD_LEFT)  targetAngle1 = max(0.0f, targetAngle1 - 5.0f);
  if (d & DPAD_RIGHT) targetAngle1 = min(180.0f, targetAngle1 + 5.0f);
  // Up/Down -> SERVO_CH2
  if (d & DPAD_UP)    targetAngle2 = max(0.0f, targetAngle2 - 5.0f);
  if (d & DPAD_DOWN)  targetAngle2 = min(180.0f, targetAngle2 + 5.0f);
}

void processControllers() {
  for (auto ctl : myControllers)
    if (ctl && ctl->isConnected() && ctl->hasData() && ctl->isGamepad())
      processGamepad(ctl);
}

void setServoAngle(int ch, float angle) {
  angle = constrain(angle, 0, 180);
  uint16_t pulse = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
  pwm.setPWM(ch, 0, pulse);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();               
  pwm.begin();
  pwm.setPWMFreq(50);         

  setServoAngle(SERVO_CH1, currentAngle1);
  setServoAngle(SERVO_CH2, currentAngle2);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
}

void loop() {
  if (BP32.update()) processControllers();

  float diff1 = targetAngle1 - currentAngle1;
  if (fabs(diff1) > maxStep)
    currentAngle1 += (diff1 > 0 ? 1 : -1) * maxStep;
  else
    currentAngle1 = targetAngle1;

  float diff2 = targetAngle2 - currentAngle2;
  if (fabs(diff2) > maxStep)
    currentAngle2 += (diff2 > 0 ? 1 : -1) * maxStep;
  else
    currentAngle2 = targetAngle2;

  setServoAngle(SERVO_CH1, currentAngle1);
  setServoAngle(SERVO_CH2, currentAngle2);

  delay(loopDelayMs);
}
