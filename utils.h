
//Includes
#include "MPU9250.h"
//Defines
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
#define L1 A1
#define L2 A0
#define R1 A2
#define R2 A3

#define SpeedA 9
#define SpeedB 10
#define trigPin 3
#define Back 5
#define Front 6

MPU9250 IMU(Wire, 0x68);



//Motors states;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void front(int ms) {
  digitalWrite(L1, LOW);
  digitalWrite(L2, HIGH);
  digitalWrite(R1, LOW);
  digitalWrite(R2, HIGH);
  delay(ms);
}
void hold(int ms) {
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
  delay(ms);
}
void right(int ms) {
  digitalWrite(L1, LOW);
  digitalWrite(L2, HIGH);
  digitalWrite(R1, HIGH);
  digitalWrite(R2, LOW);
  delay(ms);
}
void left(int ms) {
  digitalWrite(L1, HIGH);
  digitalWrite(L2, LOW);
  digitalWrite(R1, LOW);
  digitalWrite(R2, HIGH);
  delay(ms);
}
void back(int ms) {
  digitalWrite(L1, HIGH);
  digitalWrite(L2, LOW);
  digitalWrite(R1, HIGH);
  digitalWrite(R2, LOW);
  delay(ms);
}

//Get distance to object
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
int getDistance(int echoPin) {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  long distance = duration * 0.034 / 2;
  Serial.println(distance);
  return int(distance);
}

//Check distance
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
bool isSafe(int direct, int minim) {
  int dist = getDistance(direct);
  if (dist < minim || dist > 2000) {
    return false;
  }
  return true;
}
//Setspeeds
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void setSpeedLeft(int sp) {
  analogWrite(SpeedA, sp);
}
void setSpeedRight(int sp) {
  analogWrite(SpeedB, sp);
}
void setSpeedBoth(int sp) {
  analogWrite(SpeedA, sp);
  analogWrite(SpeedB, sp);
}

//Rotate 90 left
void rotateLeft90(int ms) {
  float totRads = 0;
  float ratio = ms / 1000.0;

  while (totRads > -1.33) {
    IMU.readSensor();
    left(ms);
    totRads += ratio * IMU.getGyroZ_rads();
  }
}
//Rotate 90 right
void rotateRight90(int ms) {
  float totRads = 0;
  float ratio = ms / 1000.0;

  while (totRads < 1.29) {
    IMU.readSensor();
    right(ms);
    totRads += ratio * IMU.getGyroZ_rads();
  }
}
//Rotate 180 left
void rotateLeft180(int ms) {
  float totRads = 0;
  float ratio = ms / 1000.0;

  while (totRads > -2.66) {
    IMU.readSensor();
    left(ms);
    totRads += ratio * IMU.getGyroZ_rads();
  }
}
//Rotate 180 right
void rotateRight180(int ms) {
  float totRads = 0;
  float ratio = ms / 1000.0;

  while (totRads < 2.58) {
    IMU.readSensor();
    right(ms);
    totRads += ratio * IMU.getGyroZ_rads();
  }
}


//Avoid object on left
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void avoidLeft(int ms) {
  rotateLeft90(ms);
  hold(50);
  setSpeedBoth(230);
  if (isSafe(Front, 15)) {
    front(1000);
    rotateRight90(ms);
  } else {
    rotateRight180(ms);
    if (isSafe(Front, 15)) {
      front(1000);
      rotateRight90(25);
    } else {
      rotateLeft90(ms);
      back(1000);
      rotateLeft90(ms);
    }
  }
  setSpeedBoth(255);
}

//Avoid object on right
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void avoidRight(int ms) {
  rotateRight90(ms);
  hold(50);
  setSpeedBoth(230);
  if (isSafe(Front, 15)) {
    front(1000);
    rotateLeft90(ms);
  } else {
    rotateRight180(ms);
    if (isSafe(Front, 15)) {
      front(1000);
      rotateLeft90(25);
    } else {
      rotateRight90(ms);
      back(1000);
      rotateLeft90(ms);
    }
  }
  setSpeedBoth(255);
}
