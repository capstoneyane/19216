// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPL2LIs7vzJ2"
#define BLYNK_TEMPLATE_NAME "Ya rab"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"
#include <MQ2.h>





//change this with the pin that you use
int pin = A0;
float lpg;
MQ2 mq2(pin);
SimpleTimer timer;

void setup()
{
  Serial.begin(115200);
  delay(100);

  BlynkEdgent.begin();

  

  // calibrate the device
  mq2.begin();

  
  delay(2000);
  delay(1000);
  timer.setInterval(1000L, sendSensorData);

}

void loop() {
  timer.run(); // Initiates SimpleTimer
  BlynkEdgent.run();
}
void sendSensorData()
{
  float* values = mq2.read(true); //set it false if you don't want to print the values to the Serial
  lpg = mq2.readLPG();

  Blynk.virtualWrite(V0, lpg);
  
  if (lpg > 0.000001 ) {
    Blynk.logEvent("lpg", "Alert!! Smoke detected");
   
  }
  
}


