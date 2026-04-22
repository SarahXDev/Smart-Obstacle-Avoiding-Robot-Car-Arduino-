#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial BT(9, 10); // RX, TX for Bluetooth

// Motor pins
const int IN1 = 5; // Right motor forward
const int IN2 = 6; // Right motor backward
const int IN3 = 7; // Left motor forward
const int IN4 = 8; // Left motor backward

// Ultrasonic sensor pins
const int trigPin = 2;
const int echoPin = 3;

// IR sensor
const int irSensorPin = 4;

// Servo motor
Servo myServo;
const int servoPin = 11;

// Settings
int safeDistance = 20;
bool autoMode = false;
String btCommand = ""; // Store incoming Bluetooth commands

void setup() {
  // Initialize motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  stopMoving(); // Ensure motors are off initially

  // Initialize sensors
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irSensorPin, INPUT);

  // Initialize servo
  myServo.attach(servoPin);
  myServo.write(90); // Center position

  // Initialize serial communications
  Serial.begin(9600);
  BT.begin(9600);
  
  // Send welcome message
  BT.println("Arduino Car Ready");
  BT.println("Commands:");
  BT.println("F - Forward");
  BT.println("B - Backward");
  BT.println("L - Left");
  BT.println("R - Right");
  BT.println("S - Stop");
  BT.println("A - Auto Mode");
  BT.println("Z - Spin Around");
}

void loop() {
  // Check for Bluetooth commands
  while (BT.available()) {
    char c = BT.read();
    
    // Store characters until newline
    if (c != '\n') {
      btCommand += c;
    }
    else {
      processCommand(btCommand);
      btCommand = ""; // Reset for next command
    }
  }

  // Autonomous mode operation
  if (autoMode) {
    autonomousNavigation();
  }
}

void processCommand(String command) {
  command.trim(); // Remove whitespace
  command.toUpperCase(); // Convert to uppercase
  
  Serial.print("Received command: ");
  Serial.println(command);
  BT.print("Executing: ");
  BT.println(command);

  // Process single-character commands
  if (command.length() == 1) {
    switch (command[0]) {
      case 'F': moveForward(); autoMode = false; break;
      case 'B': moveBackward(); autoMode = false; break;
      case 'L': turnLeft(); autoMode = false; break;
      case 'R': turnRight(); autoMode = false; break;
      case 'S': stopMoving(); autoMode = false; break;
      case 'A': 
        autoMode = true; 
        BT.println("Autonomous mode activated");
        break;
      case 'Z': spinAround(); autoMode = false; break;
      default:
        BT.println("Unknown command");
    }
  }
}

// Movement functions (same as before)
void moveForward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  BT.println("Moving Forward");
}

void moveBackward() {
  if (!isObstacleBehind()) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    BT.println("Moving Backward");
  } else {
    stopMoving();
    BT.println("Obstacle detected behind!");
  }
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(300); // Turn duration
  stopMoving();
  BT.println("Turned Left");
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(300); // Turn duration
  stopMoving();
  BT.println("Turned Right");
}

void stopMoving() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  BT.println("Stopped");
}

void spinAround() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(1000); // Spin duration
  stopMoving();
  BT.println("Spun Around");
}

// Sensor and autonomous functions (same as before)
bool isObstacleBehind() {
  return digitalRead(irSensorPin) == LOW;
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void autonomousNavigation() {
  long distance = getDistance();
  BT.print("Distance: ");
  BT.println(distance);

  if (distance > safeDistance) {
    moveForward();
  } else {
    stopMoving();
    delay(300);
    
    // Check left
    myServo.write(150);
    delay(300);
    long leftDist = getDistance();
    
    // Check right
    myServo.write(30);
    delay(300);
    long rightDist = getDistance();
    
    myServo.write(90); // Center
    
    if (leftDist > rightDist && leftDist > safeDistance) {
      turnLeft();
    } else if (rightDist > safeDistance) {
      turnRight();
    } else {
      moveBackward();
      delay(500);
      spinAround();
    }
  }
}