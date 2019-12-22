 /* This example shows how to use continuous mode to take
range measurements with the 6 pin VL53L0X module. It is based on
vl53l0x_ContinuousRanging_Example.c from the VL53L0X API.

The range readings are in units of mm. 

Original source: https://github.com/adafruit/Adafruit_VL53L0X
Modified by Ahmad Shamshiri for RoboJax.com
Date modified: May 31, 2018 at 19:25 at Ajax, Ontario, Canada
Watch the instruciton video for this code https://youtu.be/S2jaAQEv3Yo

Pin connection
VL53L0X Pin  Arduino Pin
VCC         5V
GND         GND
SDA         A4 or SDA if available
SCL         A5 or SCL if available
GPIO1       leave it unconnected
XSHUT       D12 (digital 12 or pin 12)
*/


#include <Wire.h>
#include <VL53L0X.h>
#include <BTS7960.h>
VL53L0X sensor;
float driverValue = 0;
int LPWM = 9; // H-bridge leg 1 ->LPWM
int enL = 8; // H-bridge enable pin 1 -> L_EN


int RPWM = 6; // H-bridge leg 2 ->RPWM
int enR = 7; // H-bridge enable pin 2 -> R_EN
int userIn = 0;
double final_data = 0;
BTS7960 motorController(enL, enR, LPWM, RPWM);
bool setMeasurementTimingBudget(200000);
void setup()
{
  
  pinMode(12,INPUT_PULLUP);
  digitalWrite(12,HIGH);
  Serial.begin(115200);
  Serial.println("Done1 ");
  Wire.begin();
  Serial.println("Done2 ");

  sensor.init();
  Serial.println("Done3 ");

  sensor.setTimeout(500);
  sensor.startContinuous();
}

void loop()
{
  motorController.Enable();
  
  if(Serial.available() > 1){
    userIn = Serial.parseInt();
  
    }
  Serial.println(userIn);
  Serial.println(driverValue);
  int distance =sensor.readRangeContinuousMillimeters();
  //int distance =sensor.startContinuous(100);
  final_data = 0.8*final_data + 0.2*distance;
  if(final_data - userIn < 1  && driverValue > -255){
    driverValue = driverValue - 0.25;
    delay(10);
  }else if (final_data - userIn > 1 && driverValue < 255) {
    driverValue = driverValue + 0.25  ;
    delay(10);
   }else {}


  motorController.TurnRight(driverValue);
  if(driverValue < 0 )
  {
    motorController.TurnLeft(-driverValue);
    }
  
 //distance = distance;
  Serial.print("Distance: ");
  Serial.print(final_data,0);
  Serial.print("mm");
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.println();

} 
