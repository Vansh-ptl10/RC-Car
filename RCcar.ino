#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// Motor driver pins (L298N/L293D)
const int enaPin = 4;   // PWM pin for speed control
const int n1 = 16;      // Direction pin 1
const int n2 = 17;      // Direction pin 2
const int enbPin = 18;
const int n3 = 19;
const int n4 = 21;

// PWM settings
const int pwmChannel = 0;        // LEDC channel 0
const int pwmChannelB = 1;
const int pwmFreq = 5000;        // 5 kHz PWM frequency
const int pwmResolution = 8;     // 8-bit resolution (0–255)
const int motorSpeed = 180;      // Fixed speed (50% duty cycle)

void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);
  
  // Initialize Bluetooth
  SerialBT.begin("ESP32Carnew");
  Serial.println("Bluetooth started, waiting for connection...");

  // Set motor control pins as outputs
  pinMode(n1, OUTPUT);
  pinMode(n2, OUTPUT);
  pinMode(enaPin, OUTPUT);
  pinMode(n3, OUTPUT);
  pinMode(n4, OUTPUT);
  pinMode(enbPin, OUTPUT);

  // Initialize PWM on enaPin
  ledcAttachChannel(enaPin, pwmFreq, pwmResolution, pwmChannel);
  ledcWrite(enaPin, motorSpeed); // Set fixed speed

  ledcAttachChannel(enbPin, pwmFreq, pwmResolution, pwmChannelB);
  ledcWrite(enbPin, motorSpeed); // Set fixed speed
}

void loop() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    Serial.print("Received command: ");
    Serial.println(cmd);

    switch (cmd) {
      case 'w': // Forward
        digitalWrite(n1, LOW);
        digitalWrite(n2, HIGH);
        digitalWrite(n4, LOW);
        digitalWrite(n3, HIGH);
        ledcWrite(enaPin, motorSpeed); // Ensure PWM is active
        ledcWrite(enbPin, motorSpeed);
        Serial.println("Motor: Forward");
        break;

      case 's': // Backward
        digitalWrite(n1, HIGH);
        digitalWrite(n2, LOW);
        digitalWrite(n4, HIGH);
        digitalWrite(n3, LOW);
        ledcWrite(enaPin, motorSpeed); // Ensure PWM is active
        ledcWrite(enbPin, motorSpeed);
        Serial.println("Motor: Backward");
        break;

      case '0': // Stop
        digitalWrite(n1, LOW);
        digitalWrite(n2, LOW);
        digitalWrite(n4, LOW);
        digitalWrite(n3, LOW);
        ledcWrite(enaPin, 0); // Disable PWM to stop motor
        ledcWrite(enbPin, 0);
        Serial.println("Motor: Stopped");
        break;
      
      case 'a': // rotate left
        digitalWrite(n1, HIGH);
        digitalWrite(n2, LOW);
        digitalWrite(n4, LOW);
        digitalWrite(n3, HIGH);
        ledcWrite(enaPin, motorSpeed); // Ensure PWM is active
        ledcWrite(enbPin, motorSpeed);
        delay(500);
        digitalWrite(n1, LOW);
        digitalWrite(n2, LOW);
        digitalWrite(n4, LOW);
        digitalWrite(n3, LOW);
        ledcWrite(enaPin, 0); // Disable PWM to stop motor
        ledcWrite(enbPin, 0);
        break;
      
      case 'd': // rotate right
        digitalWrite(n1, LOW);
        digitalWrite(n2, HIGH);
        digitalWrite(n4, HIGH);
        digitalWrite(n3, LOW);
        ledcWrite(enaPin, motorSpeed); // Ensure PWM is active
        ledcWrite(enbPin, motorSpeed);
        delay(500);
        digitalWrite(n1, LOW);
        digitalWrite(n2, LOW);
        digitalWrite(n4, LOW);
        digitalWrite(n3, LOW);
        ledcWrite(enaPin, 0); // Disable PWM to stop motor
        ledcWrite(enbPin, 0);
        break;

      default:
        Serial.println("Invalid command");
        break;
    }
  }
}

