//
//   SDL_Arduino_GroveDigitalExtender Library
//   SDL_Arduino_GroveDigitalExtender.cpp Arduino code - runs in continuous mode
//   Version 1.1
//   SwitchDoc Labs   May 26, 2015
//
//




#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "SDL_Arduino_GroveDigitalExtender.h"

/**************************************************************************/
/*!
 @brief  Sends a single command byte over I2C
 */
/**************************************************************************/
void SDL_Arduino_GroveDigitalExtender::wireWriteRegister (uint8_t reg, uint8_t value)
{
    //Serial.print("I2C Addr= 0x");
    //Serial.println(GroveDigitalExtender_i2caddr,HEX);
    //Serial.print("WriteAddress=0x");
    //Serial.print(reg, HEX);
    //Serial.print(" value=0x");
    //Serial.println(value, HEX);
    
    Wire.beginTransmission(GroveDigitalExtender_i2caddr);
#if ARDUINO >= 100

    Wire.write(reg);
    delay(100);
    // Register

    // Lower 8-bits
    Wire.write(value & 0xFF);
#else
    Wire.send(reg);                        // Register
    Wire.send(value & 0xFF);               // Lower 8-bits
#endif
    Wire.endTransmission();
}

/**************************************************************************/
/*!
 @brief  Reads a8 bit values over I2C
 */
/**************************************************************************/
void SDL_Arduino_GroveDigitalExtender::wireReadRegister(uint8_t reg, uint8_t *value)
{
    
    Wire.beginTransmission(GroveDigitalExtender_i2caddr);
#if ARDUINO >= 100
    Wire.write(reg);                       // Register
#else
    Wire.send(reg);                        // Register
#endif
    Wire.endTransmission();
    
    
    
    Wire.requestFrom(GroveDigitalExtender_i2caddr, (uint8_t)1);
    //Serial.print("Wire.available =");
    //Serial.println(Wire.available());
#if ARDUINO >= 100
    // Shift values to create properly formed integer
    *value = Wire.read();
#else
    // Shift values to create properly formed integer
    *value = Wire.receive();
#endif
}



/**************************************************************************/
/*!
 @brief  Instantiates a new SDL_Arduino_GroveDigitalExtender class
 */
/**************************************************************************/
SDL_Arduino_GroveDigitalExtender::SDL_Arduino_GroveDigitalExtender(uint8_t addr) {
    
    GroveDigitalExtender_i2caddr = addr;
    
    GroveDigitalExtender_direction = 0xff;
    GroveDigitalExtender_pullup = 0;
    GroveDigitalExtender_pulldown = 0;
    GroveDigitalExtender_interruptmask = 0;
    
    
}

/**************************************************************************/
/*!
 @brief  Setups the HW 
 */
/**************************************************************************/
void SDL_Arduino_GroveDigitalExtender::begin() {
    Wire.begin();
    
    //Serial.print("address="); Serial.println(GroveDigitalExtender_i2caddr);
    
}

/**************************************************************************/
/*!
 @brief  Reads 1 bit of GPIO
 */
/**************************************************************************/
uint8_t SDL_Arduino_GroveDigitalExtender::readGPIO(uint8_t channel) {
    uint8_t value;
    wireReadRegister(GroveDigitalExtender_REGDATA_ADDR, &value);
    uint8_t channel_Mask;
    channel_Mask = 1 << channel;
    //Serial.print("GPIO Read Raw=");
    //Serial.println(value,HEX);

    if ((channel_Mask & value) == 0)
    {
	return 0;
    }
    else
    {
	return 1;
    }
    
}

/**************************************************************************/
/*!
 @brief  Writes 1 bit of GPIO
 */
/**************************************************************************/
uint8_t SDL_Arduino_GroveDigitalExtender::writeGPIO(uint8_t channel, uint8_t value) {
    
    value = value << channel;    
    uint8_t channel_Mask;
    channel_Mask = 1 << channel;

    uint8_t old_Value;
    wireReadRegister(GroveDigitalExtender_REGDATA_ADDR, &old_Value);

    uint8_t new_Value;
    new_Value = (old_Value & ((~channel_Mask) & 0xFF)) + value;
    wireWriteRegister(GroveDigitalExtender_REGDATA_ADDR, new_Value);
    //Serial.print("GPIO Write Raw=");
    //Serial.println(value,HEX);
    
    return value;
}

/**************************************************************************/
/*!
 @brief  Reads 8 bits of GPIO
 */
/**************************************************************************/
uint8_t SDL_Arduino_GroveDigitalExtender::readGPIOByte() {
    uint8_t value;
    wireReadRegister(GroveDigitalExtender_REGDATA_ADDR, &value);
    //Serial.print("GPIO Read Raw=");
    //Serial.println(value,HEX);

    return value;
}

/**************************************************************************/
/*!
 @brief  Writes 8 bits of GPIO
 */
/**************************************************************************/
uint8_t SDL_Arduino_GroveDigitalExtender::writeGPIOByte(uint8_t value) {
   
    wireWriteRegister(GroveDigitalExtender_REGDATA_ADDR, value);
    //Serial.print("GPIO Write Raw=");
    //Serial.println(value,HEX);
    
    return value;
}

/**************************************************************************/
/*!
 @brief  Set direction of GPIO channel
 */
/**************************************************************************/
uint8_t SDL_Arduino_GroveDigitalExtender::setDirectionGPIOChannel(uint8_t channel, uint8_t direction) {
    uint8_t value;
  
    uint8_t channelMask; 
    channelMask = 1 << channel; 
    if (direction == GroveDigitalExtender_INPUT)
    {
        value = GroveDigitalExtender_direction | channelMask;
        
        
    }
    else
    {
        // assume output
        value = GroveDigitalExtender_direction & ((~channelMask) &0xFF) ;
        
    }
    
    GroveDigitalExtender_direction = value;
    
    wireWriteRegister(GroveDigitalExtender_REGDIR_ADDR, value);
    
    // Serial.print("GPIO Direction=");
    // Serial.println(value,HEX);
    
    return GroveDigitalExtender_direction;
}


/**************************************************************************/
/*!
 @brief  Set Pullup of GPIO channel
 */
/**************************************************************************/
uint8_t SDL_Arduino_GroveDigitalExtender::setPullupGPIOChannel(uint8_t channel, uint8_t state) {
    uint8_t value;
    uint8_t channelMask; 
    channelMask = 1 << channel; 
    if (state == GroveDigitalExtender_OFF)
    {
        value = GroveDigitalExtender_pullup & ((~channelMask) &0xFF) ;
    }
    else
    {
        // assume output
        value = GroveDigitalExtender_pullup | channelMask;
        
    }
    
    
    GroveDigitalExtender_pullup = value;
    wireWriteRegister(GroveDigitalExtender_REGPULLUP_ADDR, value);
    //  Serial.print("GPIO Pullup=");
    //  Serial.println(value,HEX);
    
    return GroveDigitalExtender_pullup;
}

/**************************************************************************/
/*!
 @brief  Set Pulldown of GPIO channel
 */
/**************************************************************************/
uint8_t SDL_Arduino_GroveDigitalExtender::setPulldownGPIOChannel(uint8_t channel, uint8_t state) {
    uint8_t value;
    
    uint8_t channelMask; 
    channelMask = 1 << channel; 
    if (state == GroveDigitalExtender_OFF)
    {
        value = GroveDigitalExtender_pulldown & ((~channelMask) &0xFF) ;
    }
    else
    {
        // assume output
        value = GroveDigitalExtender_pulldown | channelMask;
        
    }
    
    GroveDigitalExtender_pulldown = value;
    wireWriteRegister(GroveDigitalExtender_REGPULLDOWN_ADDR, value);
    // Serial.print("GPIO Pulldown=");
    //Serial.println(value,HEX);
    
    return GroveDigitalExtender_pulldown;
}


/**************************************************************************/
/*!
 @brief  Set Interrupt Mask of GPIO channel
 */
/**************************************************************************/
uint8_t SDL_Arduino_GroveDigitalExtender::setInterruptMaskGPIOChannel(uint8_t channel, uint8_t state) {
    uint8_t value;
    uint8_t channelMask; 
    channelMask = 1 << channel; 
    if (state == GroveDigitalExtender_OFF)
    {
        value = GroveDigitalExtender_interruptmask & ((~channelMask) &0xFF) ;
    }
    else
    {
        // assume output
        value = GroveDigitalExtender_interruptmask | channelMask;
        
    }
    
    
    GroveDigitalExtender_interruptmask = value;
    wireWriteRegister(GroveDigitalExtender_INTERRUPTMASK_ADDR, value);
    //  Serial.print("GPIO Interrupt Mask=");
    //  Serial.println(value,HEX);
    
    return GroveDigitalExtender_interruptmask;
}





 


