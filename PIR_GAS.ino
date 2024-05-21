#include <MQ2.h>
#include <Servo.h>
#include "MQ2.h"


const int mq2Pin = A0;  // Analog pin connected to the MQ-2 sensor
const int servoPin = 9;  // Digital pin connected to the SG90 servo
const int pirPin = 8;       // Digital pin connected to the PIR sensor
const int ledPin = 13;      // Digital pin connected to the LED

Servo myservo;  // Create a Servo object to control the servo


int sensorValue = 0;  // Variable to store the sensor reading
int threshold = 220;  // Threshold value for gas detection (adjust based on testing)
int pirState = LOW;       // PIR sensor state (LOW = no motion, HIGH = motion detected)



void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud
  myservo.attach(servoPin);  // Attach the servo to the specified pin
  pinMode(pirPin, INPUT); // Set PIR sensor pin as input
  pinMode(ledPin, OUTPUT); // Set LED pin as output
  Serial.println();
}

void loop() {

  pirState = digitalRead(pirPin);
  sensorValue = analogRead(mq2Pin);  // Read analog value from the MQ-2 sensor

  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);  // Print sensor value to serial monitor

  if (sensorValue > threshold) {
    Serial.println("Gas detected!");
    myservo.write(90);  // Turn the servo 90 degrees (adjust angle as needed)
    delay(10);  // Short delay for responsiveness (adjust as needed)
  } 
  
  else {
    Serial.println("No gas detected.");
    myservo.write(0);   // Turn the servo back to its initial position
    delay(10);  // Short delay (adjust as needed)
  }
  // Motion detection with LED
  if (pirState == HIGH) {
    Serial.println("Motion detected!");
    digitalWrite(ledPin, LOW);  // Turn LED on
  } 
  else {
    Serial.println("No motion detected.");
    digitalWrite(ledPin, HIGH);   // Turn LED off
  }

  
  delay(2500);
}
