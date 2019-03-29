
//Includes
#include "MPU9250.h"
//Defines
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
#define L1 A1
#define L2 A0
#define R1 A2
#define R2 A3
#define DELAY 10
#define SpeedA 9
#define SpeedB 10
#define trigPin 3
#define Back 5
#define Front 6


int cSpeed = 200;
MPU9250 IMU(Wire, 0x68);

int state = 0;

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
  digitalWrite(L1, HIGH);
  digitalWrite(L2, LOW);
  digitalWrite(R1, LOW);
  digitalWrite(R2, HIGH);
  delay(ms);
}
void left(int ms) {
  ;
  digitalWrite(L1, LOW);
  digitalWrite(L2, HIGH);
  digitalWrite(R1, HIGH);
  digitalWrite(R2, LOW);
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
  analogWrite(SpeedB, sp);
}
void setSpeedRight(int sp) {
  analogWrite(SpeedA, sp);
}
void setSpeedBoth(int sp) {
  analogWrite(SpeedA, sp);
  analogWrite(SpeedB, sp);
}

//Rotate 90 left
void rotateLeft90(int ms) {
  setSpeedBoth(max(165, min(cSpeed, 210)));
  float totRads = 0;
  float ratio = ms / 1000.0;

  while (totRads > -1.29) {
    IMU.readSensor();
    left(ms);
    totRads += ratio * IMU.getGyroZ_rads();
  }
  setSpeedBoth(cSpeed);
}
//Rotate 90 right
void rotateRight90(int ms) {
  setSpeedBoth(max(165, min(cSpeed, 210)));
  float totRads = 0;
  float ratio = ms/ 1000.0;

  while (totRads < 1.24) {
    IMU.readSensor();
    right(ms);
    totRads += ratio * IMU.getGyroZ_rads();
  }
  setSpeedBoth(cSpeed);
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
//Turn slightly right
void curveRight(int ms) {
  setSpeedRight(0);
  setSpeedLeft(min(255,cSpeed + 60));
  delay(250);
  setSpeedBoth(cSpeed);
}
//Turn slightly left
void curveLeft(int ms) {
  setSpeedRight(min(255,cSpeed + 60));
  setSpeedLeft(0);
  delay(250);
  setSpeedBoth(cSpeed);
}

//Avoid object on left
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void avoidLeft(int ms) {
  rotateLeft90(ms);
  hold(50);
  setSpeedBoth(180);
  if (isSafe(Front, 15)) {
    front(1000);
    rotateRight90(ms);
  } else {
    rotateRight180(ms);
    if (isSafe(Front, 15)) {
      front(1000);
      rotateRight90(ms);
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
  setSpeedBoth(180);
  if (isSafe(Front, 15)) {
    front(1000);
    rotateLeft90(ms);
  } else {
    rotateRight180(ms);
    if (isSafe(Front, 15)) {
      front(1000);
      rotateLeft90(ms);
    } else {
      rotateRight90(ms);
      back(1000);
      rotateLeft90(ms);
    }
  }
  setSpeedBoth(255);
}

float mapSpeed(float factor) {
  Serial.println(130.0 + factor * 125.0);
  return 130.0 + factor * 125.0;
}
int getState(String x) {

  if (x == "forward") {
    return 1;
  } else if (x == "back") {
    return 2;
  } else if (x == "right90") {
    return 3;
  } else if (x == "left90") {
    return 4;
  } else if (x == "stop") {
    return 5;
  } else if (x == "spin") {
    return 6;
  } else if (x == "right") {
    return 7;
  } else if (x == "left") {
    return 8;
  }
  else {
    int lastIdx = x.lastIndexOf('.');
    String factorS = '0' + x.substring(lastIdx);
    float factor = factorS.toFloat();
    Serial.println("new speed is " + (int)mapSpeed(factor));
    cSpeed = mapSpeed(factor);
    setSpeedBoth(cSpeed);
  }
  return -1;
}

void decodeState(int &state) {
  if (state == 1) {
    front(DELAY);
  }
  if (state == 2) {
    back(DELAY);
  }
  if (state == 3) {
    rotateRight90(DELAY);
    state = 5;
  }
  if (state == 4) {
    rotateLeft90(DELAY);
    state = 5;
  }
  if (state == 5) {
    hold(10);
  }
  if (state == 6) {
    rotateRight90(DELAY);
  }
  if (state == 7) {
    curveRight(DELAY);
  }
  if (state == 8) {
    curveLeft(DELAY);
  }
}
