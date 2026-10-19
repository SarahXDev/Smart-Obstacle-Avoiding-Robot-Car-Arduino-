# Smart Obstacle Avoiding Robot Car (Arduino)

## Overview
An Arduino-based robot car with two modes: manual control over Bluetooth and an autonomous mode that detects and avoids obstacles in real time.
Built as a university embedded systems and IoT project.


Features
Manual driving over Bluetooth with single-letter commands
Autonomous obstacle avoidance using an ultrasonic sensor (HC-SR04) on a servo that scans left and right
Rear obstacle detection with an IR sensor before reversing
Safe-distance threshold of 20 cm
Status messages sent back over Bluetooth while the robot moves


Hardware
Arduino board
Ultrasonic sensor (HC-SR04)
IR obstacle sensor
Servo motor
Bluetooth module
Motor driver (L298N) and DC motors
Pin map (from CarRobot.ino)


Component
Pins
Motors (IN1 to IN4)
5, 6, 7, 8
Ultrasonic (trig, echo)
2, 3
IR sensor
4
Servo
11
Bluetooth (RX, TX)
9, 10
Bluetooth commands
Command
Action
F
Move forward
B
Move backward (blocked if the IR sensor detects an obstacle behind)
L / R
Turn left / right
S
Stop
A
Start autonomous mode
Z
Spin around

How the autonomous mode works
The ultrasonic sensor measures the distance in front of the car.
If the path is clear (more than 20 cm), the car moves forward.
If an obstacle is detected, the car stops and the servo checks the left and right sides.
The car turns toward the side with more space.
If both sides are blocked, it reverses and spins around, then repeats.


Project structure
CarRobot.ino: full Arduino sketch
LICENSE: MIT license




Ideas for improvement
Speed control with PWM
Mobile app for control
Camera or AI-based navigation





Author
Sarah Tahsin, Computer Science graduate (Cihan University, Erbil).
GitHub: @SarahXDev