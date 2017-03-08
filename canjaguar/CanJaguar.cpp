
/*   This file is part of ArduRIO.

    ArduRIO is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ArduRIO is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ArduRIO.  If not, see <http://www.gnu.org/licenses/>. */

#include <CanJaguar.h>//ver beta 1.0
#include "Arduino.h"
#include <SoftwareSerial.h>
extern int gamemode;//used to stop all comms when the robot is disabled
extern int RX,TX;
SoftwareSerial canserial(RX,TX);
CanJaguar::CanJaguar(void){
}

void CanJaguar::Initialize(void){
canserial.begin(115200);
}

/*//////////////////////////////////////////////////////////////
what for encoding the message bytes
*///////////////////////////////////////////////////////////////


void encodebytes_fixed88(float value,int data[]){//converts 16bit floating point into 8.8 fixed point
   int integer = (int)value;//get rid of decimal place
   int decimal = (value - integer)*256;//isolate decimal place the move over two hex digits
   integer &= 0xFF;decimal &= 0xFF;//truncate -127->127 too 0->255
   int numofdata = 0;
    if(decimal == 0xFF){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFE;
        }
            else if(decimal == 0xFE){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFD;
            }
                else{
                data[numofdata++] = decimal;
                }
        
        if(integer == 0xFF){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFE;
        }
            else if(integer == 0xFE){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFD;
            }
                else{
                data[numofdata++] = integer;
                }
    }

       void encodebytes_fixed1616(float value,int data[],int afterbyte){//converts 32bit floating point into 16.16 fixed point.
       //after byte is for sending a byte after the data, if not there, just set too "false"
   int integer = (int)value;//get rid of decimal 
   int integer_8 = integer & 0xFF;//throw out last 8 bits,truncate -127->127 too 0->255
   int integer_16 = (integer >> 8) & 0xFF;//throw out first 8 bits,truncate -127->127 too 0->255
    float decimal = (value - integer)*256;//isolate all three decimal places then move over 2 hex digits
   int decimal_8 = (int)decimal;//throw out third decimal place
   int decimal_16 = (decimal - (decimal_8*10))*256;//isolate third decimal place and move over 2 hex digits, was decimal_8*10
   decimal_8 &= 0xFF;decimal_16 &= 0xFF;//truncate -127->127 too 0->255
   int numofdata = 0;
   
    if(decimal_16 == 0xFF){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFE;
        }
            else if(decimal_16 == 0xFE){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFD;
            }
                else{
                data[numofdata++] = decimal_16;
                }  
                
        if(decimal_8 == 0xFF){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFE;
        }
            else if(decimal_8 == 0xFE){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFD;
            }
                else{
                data[numofdata++] = decimal_8;
                }    
                
        if(integer_8 == 0xFF){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFE;
        }
            else if(integer_8 == 0xFE){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFD;
            }
                else{
                data[numofdata++] = integer_8;
                }
                
        if(integer_16 == 0xFF){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFE;
        }
            else if(integer_16 == 0xFE){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFD;
            }
                else{
                data[numofdata++] = integer_16;
                }
        if(afterbyte){
        data[numofdata++] = afterbyte;
        }
    }
    
  void encodebytes_fixed16(float value,int data[]){//converts 32bit floating point into 16 fixed point, INPUT MUST BE POSITVE!! 0 to 65535
   int integer = (int)value;//get rid of decimal 
   int integer_8 = integer & 0xFF;//throw out last 8 bits,truncate -127->127 too 0->255
   int integer_16 = (integer >> 8) & 0xFF;//throw out first 8 bits,truncate -127->127 too 0->255
   int numofdata = 0;
       
        if(integer_8 == 0xFF){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFE;
        }
            else if(integer_8 == 0xFE){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFD;
            }
                else{
                data[numofdata++] = integer_8;
                }
                
        if(integer_16 == 0xFF){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFE;
        }
            else if(integer_16 == 0xFE){
         data[numofdata++] = 0xFE;
         data[numofdata++] = 0xFD;
            }
                else{
                data[numofdata++] = integer_16;
                }
    }

/*///////////////////////////////////////////////////////////////////////
messages for telling the jaguar what mode it is going to be operating in.
*////////////////////////////////////////////////////////////////////////
void venable(int id,int mode[]){ 
    mode[id] = VMODE;
    byte message[] = {0xFF,0x04,id,0x00,0x02,0x02};
 canserial.write(message,sizeof(message));
}

void vcompenable(int id,int mode[]){
    mode[id] = VCMODE;
    byte message[] = {0xFF,0x04,id,0x08,0x02,0x02};
 canserial.write(message,sizeof(message));
}

void ienable(int id,int mode[]){ 
    mode[id] = IMODE;
    byte message[] = {0xFF,0x04,id,0x10,0x02,0x02};
 canserial.write(message,sizeof(message));
}

void senable(int id,int mode[]){ 
    mode[id] = SMODE;
    byte message[] = {0xFF,0x04,id,0x04,0x02,0x02};
 canserial.write(message,sizeof(message));
}

void penable(int id,int mode[],float startpoint){ 
    mode[id] = PMODE;
    int data[7] = {0};
    encodebytes_fixed1616(startpoint,data,false);
    byte message[] = {0xFF,0x08,id,0x0C,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
 canserial.write(message,sizeof(message));
}

/*/////////////////////////////////////////////////////////////
what for telling the jaguar what to do with the sensor input its receiving.
*///////////////////////////////////////////////////////////////

void CanJaguar::ConfigPIDsensor(int id,int mode,int SensorType,float value){//for potentiameter its # of turns. for encoder its turns per degree. only for PID's
//tell the jaguar what mode is using what sensor.
     if(mode == SMODE){
        byte message[] = {0xFF,0x05,0x80+id,0x05,0x02,0x02,SensorType};//store encoder in SMODE
        canserial.write(message,sizeof(message));
            }else{//implies pmode
            byte message[] = {0xFF,0x05,0x80+id,0x0D,0x02,0x02,SensorType};//store encoder in PMODE
            canserial.write(message,sizeof(message));
        }  
            int data[7] = {0};
//tell the jaguar the specs of the sensor
    if(SensorType != POTENTIOMETER){//encoder
        encodebytes_fixed16(value,data);
        byte messagec[] = {0xFF,0x06,0x40+id,0x1C,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
        canserial.write(messagec,sizeof(messagec));
    }else{//implies is potentimeter
        encodebytes_fixed16(value,data);
        byte messagec[] = {0xFF,0x06,0x80+id,0x1C,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
        canserial.write(messagec,sizeof(messagec)); 
    }
}


//for G_L use either the GREATERTHAN or LESS_THAN macro
void CanJaguar::ConfigSoftLimitForward(int id,int G_L,float value){//Configures soft limit for positive motion, for position control
int data[8] = {0};
      encodebytes_fixed1616(value,data,G_L);
       byte message[] = {0xFF,0x09,0x40+id,0x1D,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8]};//send t (g or l) then data in 16.16 
        canserial.write(message,sizeof(message));       
}
//for G_L use either the GREATERTHAN or LESS_THAN macro
void CanJaguar::ConfigSoftLimitReverse(int id,int G_L,float value){//Configures soft limit for negative motion, for position control
int data[8] = {0};
      encodebytes_fixed1616(value,data,G_L);
       byte message[] = {0xFF,0x09,0x80+id,0x1D,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8]};//send t (g or l) then data in 16.16 
        canserial.write(message,sizeof(message));       
}

void CanJaguar::EnableSoftLimit(int id){//turn on softlimit
    byte message[] = {0xFF,0x05,0x00+id,0x1D,0x02,0x02,0x01};//send boolean true
    canserial.write(message,sizeof(message)); 
}

void CanJaguar::DisableSoftLimit(int id){//turn off soft limit
        byte message[] = {0xFF,0x05,0x00+id,0x1D,0x02,0x02,0x00};//send boolean false
        canserial.write(message,sizeof(message));
}

void CanJaguar::SetBrakeCoast(int id,int mode){// for mode use BRAKE COAST JUMPER macro
        byte message[] = {0xFF,0x05,0xC0+id,0x1C,0x02,0x02,mode};
            canserial.write(message,sizeof(message)); 
}

void CanJaguar::SetMaxVout(int id,float value){
 int data[3] = {0};
  encodebytes_fixed88(value,data);
    byte message[] = {0xFF,0x06,0xC0+id,0x1D,0x02,0x02,data[0],data[1],data[2],data[3]};
    canserial.write(message,sizeof(message)); 
}

void CanJaguar::SetVramp(int id, int ramp){
int data[3] = {0};
    encodebytes_fixed16(ramp,data);
        byte message[] = {0xFF,0x06,0xC0+id,0x00,0x02,0x02,data[0],data[1],data[2],data[3]};
        canserial.write(message,sizeof(message));
}

void CanJaguar::SetVcompRamps(int id,int vramp,int vcramp){
int data[3] = {0};
    encodebytes_fixed16(vramp,data);
        byte message[] = {0xFF,0x06,0xC0+id,0x08,0x02,0x02,data[0],data[1],data[2],data[3]};
        canserial.write(message,sizeof(message)); 
        delay(10);
    encodebytes_fixed16(vcramp,data);
        byte messagea[] = {0xFF,0x06,0x00+id,0x09,0x02,0x02,data[0],data[1],data[2],data[3]};
        canserial.write(messagea,sizeof(messagea)); 
}

/*//////////////////////////////////////////////////////
what for sending constants too the jaguar for it's onboard PID.
*///////////////////////////////////////////////////////

void CanJaguar::SendConstants(int id,int mode_,float p,float i,float d){ 
    int data[7] = {0};
    if(mode_ == PMODE){
    encodebytes_fixed1616(p,data,false);
    byte messagep[] = {0xFF,0x08,0xC0+id,0x0C,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
    canserial.write(messagep,sizeof(messagep));  
     delay(10);
        encodebytes_fixed1616(i,data,false);
        byte messagei[] = {0xFF,0x08,0x00+id,0x0D,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
        canserial.write(messagei,sizeof(messagei));  
         delay(10);
            encodebytes_fixed1616(d,data,false);
            byte messaged[] = {0xFF,0x08,0x40+id,0x0D,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
            canserial.write(messaged,sizeof(messaged));  
             delay(10);       
    }
        else if(mode_ == SMODE){
        encodebytes_fixed1616(p,data,false);
        byte messagep[] = {0xFF,0x08,0xC0+id,0x04,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
        canserial.write(messagep,sizeof(messagep));  
         delay(10);
            encodebytes_fixed1616(i,data,false);
            byte messagei[] = {0xFF,0x08,0x00+id,0x05,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
            canserial.write(messagei,sizeof(messagei));  
             delay(10);
                encodebytes_fixed1616(d,data,false);
                byte messaged[] = {0xFF,0x08,0x40+id,0x05,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
                canserial.write(messaged,sizeof(messaged));  
                 delay(10);       
        }
            else if(mode_ == IMODE){
            encodebytes_fixed1616(p,data,false);
            byte messagep[] = {0xFF,0x08,0xC0+id,0x10,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
            canserial.write(messagep,sizeof(messagep));  
             delay(10);
                encodebytes_fixed1616(i,data,false);
                byte messagei[] = {0xFF,0x08,0x00+id,0x11,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
                canserial.write(messagei,sizeof(messagei));  
                 delay(10);
                    encodebytes_fixed1616(d,data,false);
                    byte messaged[] = {0xFF,0x08,0x40+id,0x11,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
                    canserial.write(messaged,sizeof(messaged));  
                     delay(10);       
        }
}
 
 
/*////////////////////////////////////////////////
what for making the motor do stuff.
*//////////////////////////////////////////////////
   
void CanJaguar::SetPower(int id,float value){
if(gamemode){//stop all comms if the robot is not enabled
    if(mode[id] != VMODE){venable(id,mode);}
        if(value>=100){value=100;}else if(value<=-100){value=-100;}
    int data[3] = {0};  
    encodebytes_fixed88(value*1.27,data);
    //encodebytes_fixed88(12,data);
    byte message[] = {0xFF,0x06,0x80+id,0x00,0x02,0x02,data[0],data[1],data[2],data[3]};  
 canserial.write(message,sizeof(message));
    }
}

void CanJaguar::SetVcomp(int id,float value){
if(gamemode){
    if(mode[id] != VCMODE){vcompenable(id,mode);}
    int data[3] = {0};  
    encodebytes_fixed88(value,data);  
    byte message[] = {0xFF,0x06,0x80+id,0x08,0x02,0x02,data[0],data[1],data[2],data[3]};
   canserial.write(message,sizeof(message));
   }
}

void CanJaguar::SetCurrent(int id,float value){
if(gamemode){
    if(mode[id] != IMODE){ienable(id,mode);}
    int data[3] = {0};  
    encodebytes_fixed88(value,data);  
    byte message[] = {0xFF,0x06,0x80+id,0x10,0x02,0x02,data[0],data[1],data[2],data[3]};
 canserial.write(message,sizeof(message));
    }
}

void CanJaguar::SetSpeed(int id,float value){
if(gamemode){
    if(mode[id] != SMODE){senable(id,mode);}
    int data[7] = {0};  
    encodebytes_fixed1616(value,data,false);  
    byte message[] = {0xFF,0x08,0x80+id,0x04,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
 canserial.write(message,sizeof(message));
    }
}

void CanJaguar::Setposition(int id,float value,float startpoint){
if(gamemode){
    if(mode[id] != PMODE){penable(id,mode,startpoint);}  
    int data[7] = {0};  
    encodebytes_fixed1616(value,data,false);
    byte message[] = {0xFF,0x08,0x80+id,0x0C,0x02,0x02,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
 canserial.write(message,sizeof(message));
    }
}

/*///////////////////////////////////////////////////////////
what for gettin stuffs from the jaguars and what not, NOT RELEASED DUE TO STRANGE VALUES ON OCASIAN, MAYBE IN THE FUTURE
*////////////////////////////////////////////////////////////

float CanJaguar::GetSpeed(int id){

}

float CanJaguar::Getposition(int id){

}

void CanJaguar::GetLimit(int id,boolean fwd,boolean rev,boolean sfwd,boolean srev){//pass the states too the for input bools
int received;
fwd = received & LIMIT_FWD;
rev = received & LIMIT_REV;
sfwd = received & LIMIT_S0FT_FWD;
srev = received & LIMIT_SOFT_REV;
}