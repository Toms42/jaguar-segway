#include "board.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <CanJaguar.h>
#include <SoftwareSerial.h>

int RX = M_RX_SOFT;
int TX = M_TX_SOFT;
CanJaguar jagnet;
int gamemode = 1;

//#define DISABLE_AUDIO

Adafruit_BNO055 bno = Adafruit_BNO055();

void setup() {
  
  beginInitTone();
  // put your setup code here, to run once:
  initAllPins();

  //digitalWrite(M_TX_SOFT,HIGH);
  
  initJaguars();
  
  Serial.begin(9600);

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);

  /* Display the current temperature */
  int8_t temp = bno.getTemp();
  Serial.print("Current Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.println("");

  bno.setExtCrystalUse(true);

  Serial.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");

  initTone();
}
/*
void loop()
{
  for(int i = 0; i < 500; i++)
  {
    jagnet.SetPower(1,40);
    delay(10);
  }
  jagnet.SetPower(1,0);
  
  for(int i = 0; i < 500; i++)
  {
    jagnet.SetPower(2,40);
    delay(10);
  }
  jagnet.SetPower(2,0);
  
  for(int i = 0; i < 500; i++)
  {
    jagnet.SetPower(3,40);
    delay(10);
  }
  jagnet.SetPower(3,0);
  
  for(int i = 0; i < 500; i++)
  {
    jagnet.SetPower(4,40);
    delay(10);
  }
  jagnet.SetPower(4,0);
  
}
*/

void loop() {
  //tone(SPEAKER, 1000);
  Serial.println("updated...");
  
  digitalWrite(LED_ORANGE, LOW);
  digitalWrite(LED_GREEN, HIGH);
  for(int i = -100; i<=100; i++)
  {
    jagnet.SetPower(1,i);
    jagnet.SetPower(2,i);
    jagnet.SetPower(3,-i);
    jagnet.SetPower(4,-i);
    delay(50);
  }
  
  digitalWrite(LED_ORANGE, HIGH);
  digitalWrite(LED_GREEN, LOW);
  
  for(int i = 100; i>=-100; i--)
  {
    jagnet.SetPower(1,i);
    jagnet.SetPower(2,i);
    jagnet.SetPower(3,-i);
    jagnet.SetPower(4,-i);
    delay(50);
  }
}

