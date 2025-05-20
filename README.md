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

## Assembly Instructions

1. **Servo Mounting**
   - Mount all servos to the base. Ensure that the yAxisArm1 is connected to the y-axis servo before assembling the left eyelid servo.
     ![image](https://github.com/user-attachments/assets/1d93d944-19df-477b-8670-e6e30568bbea)
     ![image](https://github.com/user-attachments/assets/1d799f9f-f95a-416d-8e36-4754763f77e3)



2. **Arm Connections**
   - Connect all arms to their corresponding servos.
     ![image](https://github.com/user-attachments/assets/d6fec72e-1700-4ae9-ac1c-390eb1d4d215)
   - Connect both eyes to the base, being careful not to break the flexible but fragile base pins.
     ![image](https://github.com/user-attachments/assets/b826195e-a8ce-4c46-8b42-ee7ce7dedb1c)
   - Connect [xAxisArm3.stl](3dFiles/xAxisArm3.stl) to the eyes, then [xAxisArm2.stl](3dFiles/xAxisArm2.stl) to [xAxisArm3.stl](3dFiles/xAxisArm3.stl). After centering the servos, insert the protruding part of [xAxisArm2.stl](3dFiles/xAxisArm2.stl) into the hole in [xAxisArm1.stl](3dFiles/xAxisArm1.stl).
     ![image](https://github.com/user-attachments/assets/273d0b4d-0dfd-4f0b-8072-3e227bf115e5)
     ![image](https://github.com/user-attachments/assets/a9b26eb4-4221-4303-b7a5-53b1ead345e7)
     ![image](https://github.com/user-attachments/assets/e998718d-3480-4deb-b451-8e8c84682ace)
   - Connect [yAxisArm3.stl](3dFiles/yAxisArm3.stl) to [yAxisArm2.stl](3dFiles/yAxisArm2.stl), then [yAxisArm2.stl](3dFiles/yAxisArm2.stl) to [xAxisArm1.stl](3dFiles/xAxisArm1.stl) (which should already be connected to the servo). Be cautious as these parts are flexible but can break under tension.
   ![image](https://github.com/user-attachments/assets/9ce8e224-6201-4925-9436-2e68799ed5d8)
   ![image](https://github.com/user-attachments/assets/65009812-b63b-4e7b-b352-546f0df2ccdd)

3. **Eyelid Assembly**
   - Connect the eyelids to both the left and right eyes. The supporting part is designed to be flexible but can bend or break under excessive tension.
   - Attach the upper and lower arms to the eyelids using the corresponding screws (long screw for the lower eyelid arm, short screw for the upper eyelid arm). First attach the upper arm, then the lower arm.
   - For easier assembly, first attach the arms to the eyelids, then connect them to the base.
   - Connect the [eyeLidArmLeft.stl](3dFiles/eyeLidArmLeft.stl) and [eyeLidArmRight.stl](3dFiles/eyeLidArmRight.stl) to the eyelidarms using the [eyeLidScrew.stl](3dFiles/eyeLidScrew.stl).
     ![image](https://github.com/user-attachments/assets/464294e3-3717-42d6-bb62-d61231fca179)
     ![image](https://github.com/user-attachments/assets/2499c693-6b68-42b3-9093-82630428090e)
     ![image](https://github.com/user-attachments/assets/4e54b646-fc87-4b90-badf-236bcdb18f53)


4. **Eyelid Servo Calibration**
   - **Important Note:** The arm/horn mounted on the eyelid servo should not be perpendicular to the servo itself.
   - **Calibration Steps:**
     - Ensure all servos are calibrated to 90 degrees using `servo_calibration.ino`.
     - Close both eyelids.
     - Mount the arm on the servo (eyelidarmleft/right) and slide it until the eyelids are closed. This should not move the motor; if it does, recalibrate the servo.
     - Repeat the process for the other eye.

5. **Final Adjustments**
   - The assembly is designed for a tight, snap-fit connection. Only the x and y axis arms require screws for the horns.
   - Ensure all connections are secure and the mechanism moves smoothly without additional parts.
     ![image](https://github.com/user-attachments/assets/1b2ee45e-a18f-4694-9390-3224901ffbba)
     ![image](https://github.com/user-attachments/assets/34ea9faa-6ab1-4927-8eac-fc00e98bc3f8)

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
