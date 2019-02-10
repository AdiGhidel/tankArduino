
#include "utils.h"
int status;

void setup() {
  // put your setup code here, to run once:
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(SpeedA, OUTPUT);
  pinMode(SpeedB, OUTPUT);
  setSpeedBoth(255);
  //set the distance sensor
  
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(Back, INPUT); // Sets the Back as an Input
  pinMode(Front, INPUT); // Sets the Front as an Input
  
  Serial.begin(115200); // Starts the serial communication
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
  front(50);
  if (!isSafe(Front,15)) {
     if(rand()%2 == 1) {
        avoidLeft(25);    
     } else {
        avoidRight(25);
     }
     
  }
  digitalWrite(LED_BUILTIN, LOW);
}
