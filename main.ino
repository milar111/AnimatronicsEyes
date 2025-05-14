#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Bluepad32.h>

Adafruit_PWMServoDriver pwm;

// Eye limits and mid‑points
const int xLeft  = 130;
const int xRight =  70;
const int xMid   =  95;

const int yUp    =  70;
const int yDown  = 110;
const int yMid   = (yUp + yDown) / 2;  // 90

// Sweep radius
const int RADIUS = min(
  min(abs(xLeft - xMid), abs(xMid - xRight)),
  min(abs(yMid - yUp),    abs(yDown - yMid))
); // =20

// Servo channels
#define EYE_X     0
#define EYE_Y     1
#define LID_LEFT  2
#define LID_RIGHT 3

#define SERVO_MIN 102
#define SERVO_MAX 512

ControllerPtr controllers[BP32_MAX_GAMEPADS];

// animation state machine
bool susActive    = false;
int  sweepDir     = +1;   // +1 = left=>right, –1 = right=>left
int  passesLeft   = 0;
unsigned long stateStart = 0;
enum SusState { IDLE, PAUSE_END, CENTER_HOLD, SWEEP } susState = IDLE;

void onConnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (!controllers[i]) {
      controllers[i] = ctl;
      break;
    }
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (controllers[i] == ctl) {
      controllers[i] = nullptr;
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

  // center eyes, open lids
  setServo(EYE_X, xMid);
  setServo(EYE_Y, yMid);
  setServo(LID_LEFT,  50);
  setServo(LID_RIGHT, 90);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
}

void loop() {
  BP32.update();

  // detect triangle hold
  bool tri = false;
  for (auto ctl : controllers) {
    if (ctl && ctl->isConnected()) {
      tri = ctl->y();
      break;
    }
  }

  // enter/exit animation
  if (tri && !susActive) {
    susActive  = true;
    passesLeft = random(3,5);
    sweepDir   = +1;
    susState   = PAUSE_END;
    stateStart = millis();
  } else if (!tri && susActive) {
    susActive = false;
    susState  = IDLE;
    setServo(EYE_X, xMid);
    setServo(EYE_Y, yMid);
    setServo(LID_LEFT,  50);
    setServo(LID_RIGHT, 90);
  }

  if (susActive) {
    unsigned long dt = millis() - stateStart;

    switch (susState) {
      case PAUSE_END:
        setServo(EYE_X, sweepDir>0 ? xLeft : xRight);
        setServo(EYE_Y, yMid);
        setServo(LID_LEFT,  70);
        setServo(LID_RIGHT, 70);
        if (dt > 800) {
          susState   = SWEEP;
          stateStart = millis();
        }
        break;

      case SWEEP: {
        float frac = float(dt) / 5000.0f;
        if (frac > 1) frac = 1;
        int angleX = (sweepDir>0)
          ? xLeft  + round(frac * (xRight - xLeft))
          : xRight + round(frac * (xLeft  - xRight));
        setServo(EYE_X, angleX);
        setServo(EYE_Y, yMid);

        if (dt >= 5000) {
          sweepDir *= -1;
          passesLeft--;
          susState   = (passesLeft>0) ? PAUSE_END : CENTER_HOLD;
          stateStart = millis();
        }
        break;
      }

      case CENTER_HOLD:
        setServo(EYE_X, xMid);
        setServo(EYE_Y, yMid);
        setServo(LID_LEFT,  60);
        setServo(LID_RIGHT, 60);
        if (dt > 1500) {
          // restart
          passesLeft = random(2,3);
          sweepDir   = +1;
          susState   = PAUSE_END;
          stateStart = millis();
        }
        break;

      default:
      case IDLE:
        break;
    }
  }
  else {
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
      int angleY = yMid - round(jy * RADIUS);

      setServo(EYE_X, angleX);
      setServo(EYE_Y, angleY);
      setServo(LID_LEFT,  ctl->throttle() > 0 ? 90 : 50);
      setServo(LID_RIGHT, ctl->brake()    > 0 ? 50 : 90);
      break;
    }
  }
}
