//
// SDL_Arduino_GroveDigitalExtender_Test.ino
// SwitchDoc Labs
// August 2015
// http://www.switchdoc.com/8-gpio-i2c-extender-board/


#include <Wire.h>

#include "SDL_Arduino_GroveDigitalExtender.h"


SDL_Arduino_GroveDigitalExtender GDE;


void setup() {
  
  
    Wire.begin();

    Serial.begin(9600);
    // put your setup code here, to run once:
  
  
    byte error, address;
    int nDevices;
  
    Serial.println("");
    Serial.println("--------------------------");
    Serial.println("SDL_Arduino_GroveDigitalExtender_Test");
    Serial.println("Version 1.1");
/*
    nDevices = 0;
    for(address = 1; address < 127; address++ ) 
    {
      // The i2c_scanner uses the return value of
      // the Write.endTransmisstion to see if
      // a device did acknowledge to the address.
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
  
      if (error == 0)
      {
        Serial.print("I2C device found at address 0x");
        if (address<16) 
          Serial.print("0");
        Serial.print(address,HEX);
        Serial.println("");
  
        nDevices++;
      }
      else if (error==4) 
      {
        Serial.print("Unknow error at address 0x");
        if (address<16) 
          Serial.print("0");
        Serial.println(address,HEX);
      }    
    }
    if (nDevices == 0)
      Serial.println("No I2C devices found\n");
    else
      Serial.println("done\n");
*/
}

void loop() {
 
  // blink IO 0 to see LED
  
  GDE.setDirectionGPIOChannel(0, GroveDigitalExtender_OUTPUT);  
  //GDE.setDirectionGPIOChannel(1, GroveDigitalExtender_OUTPUT);
  GDE.setDirectionGPIOChannel(2, GroveDigitalExtender_OUTPUT);
  //GDE.setDirectionGPIOChannel(3, GroveDigitalExtender_OUTPUT);
  //GDE.setPullupGPIOChannel(0, GroveDigitalExtender_ON);
  //GDE.setPulldownGPIOChannel(0, GroveDigitalExtender_ON);
  byte value;
    
  while (1)
  {
    
    // loop about on IO 0 to see the LED blink
    
    Serial.println("----------------");
    GDE.writeGPIO(0,0x01);
    GDE.writeGPIO(2,0x01);
    delay(1000);
    Serial.println("++++++++++++++");
    value = GDE.readGPIO(0);
    Serial.print("GPIO Value =");
    Serial.println(value, HEX);
    
    GDE.writeGPIO(0,0x00);
    GDE.writeGPIO(2,0x00);
    delay(1000);
    Serial.println("----------------");
    Serial.println("----------------");
   
    
  }
  

}
