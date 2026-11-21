#include <Servo.h>

Servo myServo;

// Motor pins (L298N)
#define IN1 7
#define IN2 8

// Sensors
#define IR 2
#define TRIG 3
#define ECHO 4

long duration;
int distance;

void setup() {
  Serial.begin(9600);

  // Servo
  myServo.attach(9);

  // Motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Sensors
  pinMode(IR, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Initial state
  myServo.write(0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void loop() {

  // 🔍 IR SENSOR CHECK
  int wasteDetected = digitalRead(IR);

  if (wasteDetected == LOW) {   // Object detected

    Serial.println("Waste Detected");

    delay(1000);

    // 🔄 Rotate servo (simulate sorting)
    myServo.write(90);
    delay(2000);

    // ⚙️ Run motor (compression)
    runMotor();

    // 🔄 Return servo to original
    myServo.write(0);
    delay(2000);
  }

  // 📏 Check bin level
  checkLevel();

  delay(500);
}

// 🔧 MOTOR FUNCTION
void runMotor() {
  Serial.println("Motor ON");

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(3000);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  Serial.println("Motor OFF");
}

// 📏 ULTRASONIC FUNCTION
void checkLevel() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 10) {
    Serial.println("⚠️ Bin Full!");
  }
}
