
#include "utils.h"
#include <SoftwareSerial.h>
int status;
SoftwareSerial blueTooth(8, 7);
#include <IRremote.h>

const int RECV_PIN = 0;
IRrecv irrecv(RECV_PIN);
decode_results results;

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
  irrecv.enableIRIn();
  irrecv.blink13(true);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    if (irrecv.decode(&results)){
        long long x = results.value;
        irrecv.resume();
        if (x == 0xF7609F) {
          front(50);
        } else if (x == 0xF7906F) {
          rotateLeft90(50); 
        } else if (x == 0xF7D02F) {
          rotateRight90(50);
        } else if (x == 0xF750AF) {
          hold(50);
        } else if (x == 0xF7708F)  {
          back(50);
        }

        
        
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
