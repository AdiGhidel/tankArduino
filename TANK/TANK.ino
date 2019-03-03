
#include "utils.h"
#include <SoftwareSerial.h>
int status;

SoftwareSerial blueTooth(0, 1);
#include <IRremote.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(SpeedA, OUTPUT);
  pinMode(SpeedB, OUTPUT);
  setSpeedBoth(200);
  //set the distance sensor
  
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(Back, INPUT); // Sets the Back as an Input
  pinMode(Front, INPUT); // Sets the Front as an Input
  
  Serial.begin(9600); // Starts the serial communication
  blueTooth.begin(9600);

  while (!Serial) {}

  //start the sensor
  status = IMU.begin();

  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1) {}
  }

}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    blueTooth.write("1");
    delay(500);
    if (Serial.available()){
      digitalWrite(LED_BUILTIN, LOW);
      char c = Serial.read();
      Serial.write(c);
      blueTooth.write(c);
      char x = blueTooth.read();
      Serial.write(x);
      delay(1000);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    
//  if (!isSafe(Front,15)) {
////     if(rand()%2 == 1) {
//        rotateLeft90(10);    
////     } else {
////        rotateRight90(10);
////     }
//     
//  }

}
