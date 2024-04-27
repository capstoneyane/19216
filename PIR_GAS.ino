#define DHTPIN 2 // Digital pin connected to the DHT sensor (change as needed)
#define DHTTYPE DHT11 // DHT sensor type (DHT11 or DHT22)

#include <MQ2.h>
#include <Servo.h>
#include <math.h>  // Include math library for smooth movement function
#include <LiquidCrystal.h> // Include LiquidCrystal library for LCD
#include "MQ2.h"


const int mq2Pin = A0;  // Analog pin connected to the MQ-2 sensor
const int servoPin = 9;  // Digital pin connected to the SG90 servo
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // LCD pin connections (adjust as needed)
const int pirPin = 8;       // Digital pin connected to the PIR sensor
const int ledPin = 13;      // Digital pin connected to the LED
const int dhtPin = 2;        // Digital pin connected to the DHT sensor


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);  // Create LiquidCrystal object
Servo myservo;  // Create a Servo object to control the servo

int targetPosition = 90;  // Desired final position (adjust as needed)
int currentPosition = myservo.read();  // Get current servo position
const double speed = 0.2;  // Adjust this value for desired movement speed (smaller = slower)
int sensorValue = 0;  // Variable to store the sensor reading
int threshold = 600;  // Threshold value for gas detection (adjust based on testing)
int pirState = LOW;       // PIR sensor state (LOW = no motion, HIGH = motion detected)

DHT dht(DHTPIN, DHTTYPE); // Create a DHT sensor object


// Function to calculate smooth movement with speed parameter
int smoothMoveToPosition(int targetPos, int currentPos, double speed) {
  int difference = targetPos - currentPos;
  double maxStep = abs(difference) * speed;
  int step = difference > 0 ? min(difference, (int)maxStep) : max(difference, -(int)maxStep);
  int newPosition = currentPos + step;
  return constrain(newPosition, 0, 180);
}

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud
  myservo.attach(servoPin);  // Attach the servo to the specified pin
  lcd.begin(16, 2);      // Initialize LCD (16 columns, 2 rows)
  lcd.clear();          // Clear LCD display
  pinMode(pirPin, INPUT); // Set PIR sensor pin as input
  pinMode(ledPin, OUTPUT); // Set LED pin as output
  Serial.println();
  Serial.println("Status\tTemperature (C)\t(F)");
  dht.begin();           // Initialize DHT sensor
  
  
}

void loop() {

  // Delay between readings based on sensor specifications (adjust as needed)
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.println(dht.toFahrenheit(temperature), 1);

  pirState = digitalRead(pirPin);
  // Update LCD display every 2 seconds
 if (millis() % 2000 == 0) {
    lcd.clear();
    lcd.print("Sensor Value:");
    lcd.setCursor(0, 1);
    lcd.print(sensorValue);
    
  }
 sensorValue = analogRead(mq2Pin);  // Read analog value from the MQ-2 sensor

  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);  // Print sensor value to serial monitor

  if (sensorValue > threshold) {
    Serial.println("Gas detected!");
    lcd.clear();
    lcd.print("Gas Detected!");
    // Move servo smoothly
    currentPosition = smoothMoveToPosition(targetPosition, currentPosition, speed);
    myservo.write(currentPosition);
    delay(10);  // Short delay for responsiveness (adjust as needed)
  } else {
    Serial.println("No gas detected.");
    lcd.clear();
    lcd.print("No Gas Detected");
    // Move servo back to initial position smoothly
    currentPosition = smoothMoveToPosition(0, currentPosition, speed);
    myservo.write(currentPosition);
    delay(10);  // Short delay (adjust as needed)
  }
  // Motion detection with LED
  if (pirState == HIGH) {
    Serial.println("Motion detected!");
    lcd.setCursor(0, 1);
    lcd.print("Motion!");
    digitalWrite(ledPin, HIGH);  // Turn LED on
  } else {
    Serial.println("No motion detected.");
    lcd.setCursor(0, 1);
    lcd.print("        ");  // Clear "Motion!" text
    digitalWrite(ledPin, LOW);   // Turn LED off
  }

  
  delay(20);
}
