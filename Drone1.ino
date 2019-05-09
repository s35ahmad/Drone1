
#include <Wire.h>
#include "BMP085.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Servo.h>


BMP085 pressure_m;
MPU6050 accelgyro;
int ax, ay, az;
long ax0, ax1, ax2, ax3, ax4, ax5, axCalib;
long ay0, ay1, ay2, ay3, ay4, ay5, ayCalib;
long az0, az1, az2, az3, az4, az5, azCalib;
long esc_1, esc_2, esc_3, esc_4;
long throttle = 0, scale = 50;
long battery_voltage;
int gx, gy, gz;
#define LED_PIN 13
long low_battery = 170;
long descend = 1100;
bool blinkState = false;
Servo ESC1, ESC2;  //Create as much as Servoobject you want. You can controll 2 or more Servos at the same time
Servo ESC3, ESC4;  //Create as much as Servoobject you want. You can controll 2 or more Servos at the same time

int value = 0; 
void setup(){
  pinMode(3, INPUT);    
  pinMode(2, INPUT);  
  ESC1.attach(9);    // attached to pin 9 I just do this with 1 Servo
  ESC2.attach(10);    // attached to pin 9 I just do this with 1 Servo
  ESC3.attach(11);    // attached to pin 9 I just do this with 1 Servo
  ESC4.attach(12);    // attached to pin 9 I just do this with 1 Servo

  Serial.begin(9600);
  Wire.begin();
 Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);
  pressure_m.bmp085Calibration();
}

void loop()
{
 // float temperature = pressure_m.bmp085GetTemperature(); //MUST be called first
 // float pressure = pressure_m.bmp085GetPressure();
  //float altitude = pressure_m.calcAltitude(pressure);

  //Serial.print("Temperature: ");
 // Serial.print(temperature, 2); //display 2 decimal places
  //Serial.println("deg C");

  //Serial.print("Pressure: ");
  //Serial.print(pressure, 0); //whole number only.
  //Serial.println(" Pa");

  //Serial.print("Altitude: ");
  //Serial.print(altitude, 2); //display 2 decimal places
  //Serial.println(" M");

 // Serial.println();//line break

 // delay(100); //wait a second and get values again.
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
axCalib = 71;
//ax5 = ax4;
//ax4 = ax3;
ax3 = ax2;
ax2 = ax1;
ax1 = ax;
ax0 = (ax1+ax2+ax3)/3 - axCalib;

ayCalib = -170;
//ay5 = ay4;
//ay4 = ay3;
ay3 = ay2;
ay2 = ay1;
ay1 = ay;
ay0 = (ay1+ay2+ay3)/3 - ayCalib;

azCalib = -17253;
//az5 = az4;
//az4 = az3;
az3 = az2;
az2 = az1;
az1 = az;
az0 = (az1+az2+az3)/3 - azCalib;

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    // display tab-separated accel/gyro x/y/z values
    Serial.print("a/g:\t");
    Serial.print(ax0); Serial.print("\t");
    Serial.print(ay0); Serial.print("\t");
    Serial.print(az0); Serial.print("\t");
    //Serial.print(gx+100); Serial.print("\t");
    //Serial.print(gy+34); Serial.print("\t");
    //Serial.println(gz+67);
throttle = (analogRead(2))/5+900;
    Serial.print(throttle); Serial.print("\t");
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    esc_1 = throttle - ax0/scale + ay0/scale;        //Calculate the pulse for esc 1 (front-right - CCW).
    esc_2 = throttle + ax0/scale + ay0/scale;        //Calculate the pulse for esc 2 (rear-right - CW).
    esc_3 = throttle + ax0/scale - ay0/scale;        //Calculate the pulse for esc 3 (rear-left - CCW).
    esc_4 = throttle - ax0/scale - ay0/scale;        //Calculate the pulse for esc 4 (front-left - CW).

    battery_voltage = ((float)analogRead(3));
    Serial.print(battery_voltage); Serial.print("\t");
//if(battery_voltage > 6.0 && battery_voltage < low_battery){
   // ESC1.writeMicroseconds(descend);
  //  ESC2.writeMicroseconds(descend);
 //   ESC3.writeMicroseconds(descend);
 //   ESC4.writeMicroseconds(descend);
//}
    ESC1.writeMicroseconds(esc_1);
    ESC2.writeMicroseconds(esc_2);
    ESC3.writeMicroseconds(esc_3);
    ESC4.writeMicroseconds(esc_4);
    Serial.print(esc_1); Serial.print("\t");
    Serial.print(esc_2); Serial.print("\t");
    Serial.print(esc_3); Serial.print("\t");
    Serial.println(esc_4);
  delay(100);
  if(Serial.available()) 
    throttle = Serial.parseInt();    // Parse an Integer from Serial
}
