# Animatronic Eyes Project

A servo-controlled animatronic eyes system featuring realistic movements and animations, controlled via Bluetooth gamepad (PS4 controller).

## Overview

This project creates lifelike animatronic eyes using servo motors for X/Y movement and eyelid control, powered by an ESP32 microcontroller and controllable through a Bluetooth gamepad (PS4 controller).

## Features

- Full X/Y axis eye movement control
- Independent left/right eyelid control
- Bluetooth gamepad (PS4 controller) integration
- "Sus mode" animation sequence
- Servo calibration utilities
- 3D printable mechanical components

## Hardware Requirements

- 3D printed components (files provided)
- 1x ESP32 development board
- 1x PCA9685 PWM servo driver
- 4x micro servo motors
- 1x Bluetooth gamepad (PS4 controller)
- 1x power supply
- Wires

## Wiring Instructions

Connect ESP32 to PCA9685 (left side pins):
```
ESP32      -> PCA9685
GND        -> GND
GPIO22(SCL)-> SCL
GPIO21(SDA)-> SDA
VCC        -> V+ (on PCA9685 left side)
```

To power the servos and board:
- Connect the positive wire from the battery to V+ (green terminal block in the middle of PCA9685)
- Connect the negative wire from the battery to V- (other terminal in the green block)

## Servo Channel Mapping
```
Channel 0: Eye X-axis
Channel 1: Eye Y-axis
Channel 2: Left Eyelid
Channel 3: Right Eyelid
```

## Software Requirements

- Arduino IDE
- Required Libraries:
  - Wire.h
  - Adafruit_PWMServoDriver
  - Bluepad32

## Setup Guide

1. **Install Software**
   - Install Arduino IDE
   - Add ESP32 board support to Arduino IDE
   - Install required libraries: Adafruit PWM Servo Driver Library, Bluepad32

2. **Initial Hardware Setup**
   - Wire the ESP32 and PCA9685 as described above.

3. **Servo Calibration**
   - Connect the first servo to Channel 0:
     ```
     PCA9685    -> Servo
     V+         -> Red (5V)
     GND        -> Brown (GND)
     Channel 0  -> Orange (Signal)
     ```
   - Upload `servo_calibration.ino` to center all servos at 90° for initial alignment.

4. **Servo Range Test**
   - Upload `angle_tester.ino` and open Serial Monitor (115200 baud).
   - Test each servo channel:
     - Channel 0 (X-axis): Find left/right limits (typically 70°-130°)
     - Channel 1 (Y-axis): Find up/down limits (typically 70°-110°)
     - Channel 2-3 (Eyelids): Test open/close (see below for values)
   - The default values provided below should work for most setups. If you experience issues, use the angle_tester to determine your own safe ranges and update them in `main.ino`.

5. **Software Configuration**
   - Adjust servo limits in `main.ino` as needed:
   ```cpp
   const int xLeft  = 130;  // Your tested left limit
   const int xRight = 70;   // Your tested right limit
   const int xMid   = 95;   // Your center position

   const int yUp    = 70;   // Your tested up limit
   const int yDown  = 110;  // Your tested down limit
   ```

6. **Final Assembly**
   - [assembly instructions coming soon]

7. **Final Testing**
   - Upload `main.ino`
   - Pair your Bluetooth gamepad (PS4 controller)
   - Test controls:
     - Left analog stick: Eye X/Y movement
     - L2/R2 triggers: Left/right eyelid control
     - Triangle button: "Sus mode" animation

## Troubleshooting

### Servo Issues
- Jittering: Check power supply (2A minimum)
- Binding: Verify servo limits
- Unresponsive: Check wiring connections

### Control Issues
- No gamepad (PS4 controller) response: Check battery/pairing
- Erratic movement: Verify servo limits
- Strange behavior: Check channel assignments

## Reference Values

### Default Servo Ranges
```
X-axis: 70° to 130° (center: 95°)
Y-axis: 70° to 110° (center: 90°)
Eyelids: Left: 50° (closed) to 90° (open)
         Right: 50° (open) to 90° (closed)
```