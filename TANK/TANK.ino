
#include "utils.h"
#include <SoftwareSerial.h>
int status;

SoftwareSerial blueTooth(0, 1);

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
  blueTooth.begin(115200);

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
  blueTooth.setTimeout(100);
}
int state = 0;
void loop() {
  
    blueTooth.flush();  
    digitalWrite(LED_BUILTIN, HIGH);
    if (blueTooth.available()){
      String x = blueTooth.readString();
      Serial.println(x);
      digitalWrite(LED_BUILTIN, HIGH);
      int newstate = getState(x);
      if(newstate != -1) {
        state = newstate;
      }
      Serial.println(state);
    }
    decodeState(state);
}
