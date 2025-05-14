#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Bluepad32.h>

Adafruit_PWMServoDriver pwm;

// Eye limits and mid‐points
const int xLeft  = 130;
const int xRight =  70;
const int xMid   =  95;

const int yUp    =  70;
const int yDown  = 110;
const int yMid   = (yUp + yDown) / 2;  //  90

// radius
const int RADIUS = min( min(abs(xLeft - xMid), abs(xMid - xRight)), min(abs(yMid - yUp),   abs(yDown - yMid)) ); // =20

// Servo channels
#define EYE_X     0
#define EYE_Y     1
#define LID_LEFT  2
#define LID_RIGHT 3

#define SERVO_MIN 102
#define SERVO_MAX 512

ControllerPtr controllers[BP32_MAX_GAMEPADS];

void onConnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (!controllers[i]) {
      controllers[i] = ctl;
      Serial.print("Controller connected @ slot ");
      Serial.println(i);
      break;
    }
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (controllers[i] == ctl) {
      controllers[i] = nullptr;
      Serial.print("Controller disconnected @ slot ");
      Serial.println(i);
      break;
    }
  }
}

void setServo(int ch, int angle) {
  angle = constrain(angle, 0, 180);
  uint16_t pulse = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
  pwm.setPWM(ch, 0, pulse);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(50);
  delay(200);

  setServo(EYE_X, xMid);
  setServo(EYE_Y, yMid);
  setServo(LID_LEFT,  50);
  setServo(LID_RIGHT, 90);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();

  Serial.println("Ready. Left stick = eyes; R2 closes left lid; L2 closes right lid.");
}

void loop() {
  BP32.update();

  for (auto ctl : controllers) {
    if (!ctl || !ctl->isConnected()) continue;

    float jx = ctl->axisX() / 512.0f;
    float jy = ctl->axisY() / 512.0f;
    const float DZ = 0.10f;
    if (fabs(jx) < DZ) jx = 0;
    if (fabs(jy) < DZ) jy = 0;

    float mag = sqrt(jx*jx + jy*jy);
    if (mag > 1.0f) { jx /= mag; jy /= mag; }

    int angleX = xMid + round(jx * RADIUS);
    int angleY = yMid - round(jy * RADIUS);  // invert Y

    setServo(EYE_X, angleX);
    setServo(EYE_Y, angleY);

    //  eyelids 
    // throttle() is R2, brake() is L2
    if (ctl->throttle() > 0) {
      // close 
      setServo(LID_LEFT, 90);
    } else {
      // release 
      setServo(LID_LEFT, 50);
    }

    if (ctl->brake() > 0) {
      // close 
      setServo(LID_RIGHT, 50);
    } else {
      // release
      setServo(LID_RIGHT, 90);
    }

    break; 
  }

}