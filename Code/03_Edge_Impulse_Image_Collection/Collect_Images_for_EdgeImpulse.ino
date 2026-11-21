#include <Servo.h>
#include <SoftwareSerial.h>

Servo myServo;
SoftwareSerial espSerial(10, 11); // RX, TX

#define IN1 7
#define IN2 8
#define IR 2

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  myServo.attach(9);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IR, INPUT);

  myServo.write(90); // start center
}

void loop() {

  // Detect waste
  if (digitalRead(IR) == LOW) {

    Serial.println("Waste detected");
    delay(1000);

    // Wait for ESP32 data
    unsigned long startTime = millis();
    while (!espSerial.available()) {
      if (millis() - startTime > 5000) {
        Serial.println("No data from ESP");
        return;
      }
    }

    char wasteType = espSerial.read();

    Serial.print("Type: ");
    Serial.println(wasteType);

    // Servo positions for 4 bins
    if (wasteType == '1') myServo.write(0);     // Dry
    else if (wasteType == '2') myServo.write(60);   // Wet
    else if (wasteType == '3') myServo.write(120);  // Metal
    else if (wasteType == '4') myServo.write(180);  // Other

    delay(2000);

    // Run motor (compression)
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    delay(3000);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    delay(1000);

    // Reset servo
    myServo.write(90);
  }
}