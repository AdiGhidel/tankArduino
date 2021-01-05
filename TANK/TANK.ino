
#include "utils.h"
#include <SoftwareSerial.h>
#include "PS2X_lib.h"

int status;
byte vibrate = 0;
PS2X ps2x; // create PS2 Controller Class
SoftwareSerial blueTooth(0, 1);

void setup()
{
  // put your setup code here, to run once:
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(SpeedA, OUTPUT);
  pinMode(SpeedB, OUTPUT);
  setSpeedBoth(cSpeed);
  //set the distance sensor

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(Back, INPUT);     // Sets the Back as an Input
  pinMode(Front, INPUT);    // Sets the Front as an Input

  Serial.begin(57600); // Starts the serial communication
  blueTooth.begin(115200);

  while (!Serial)
  {
  }

  //start the sensor
  status = IMU.begin();

  if (status < 0)
  {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1)
    {
    }
  }
  blueTooth.setTimeout(100);
  delay(500);
  ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, false, false);
  if (error == 0)
  {
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
    if (pressures)
      Serial.println("true ");
    else
      Serial.println("false");
    Serial.print("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");
  }
  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  type = ps2x.readType();
  switch (type)
  {
  case 0:
    Serial.println("Unknown Controller type found ");
    break;
  case 1:
    Serial.println("DualShock Controller found ");
    break;
  case 2:
    Serial.println("GuitarHero Controller found ");
    break;
  case 3:
    Serial.println("Wireless Sony DualShock Controller found ");
    break;
  }
}

void loop()
{

  ps2x.read_gamepad(false, vibrate);
  //    blueTooth.flush();
  //    digitalWrite(LED_BUILTIN, HIGH);
  //    if (blueTooth.available()){
  //      String x = blueTooth.readString();
  //      Serial.println(x);
  //      digitalWrite(LED_BUILTIN, HIGH);
  //      int newstate = getState(x);
  //      if(newstate != -1) {
  //        state = newstate;
  //      }
  //      Serial.println(state);
  //    }
  //    decodeState(state);
  int LSpeed = ps2x.Analog(PSS_LY);
  int SL = -(LSpeed - 128);

  int RSpeed = ps2x.Analog(PSS_RY);
  int SR = -(RSpeed - 128);
  setSpeedRight(abs(SR) + 40);
  setSpeedLeft(abs(SL) + 40);
  Serial.println(SR);
  Serial.println(SL);
  if (SL > 10 || SR > 10)
  {
    front(50);
  }
  if (SL < -10 || SR < -10)
  {
    back(50);
  }
  delay(5);
}
