
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
    
#ifndef CanJaguar_H_
#define CanJaguar_H_//ver beta 1.0
#include <SoftwareSerial.h>
#include <Arduino.h>

/*/////////////////////////////////////////////////////////////////////////
if tx,rx pins haven't been explicitly defined, set these defaults.
*//////////////////////////////////////////////////////////////////////////


/*//////////////////////////////////////////////////////////////////
API sensor definitions for closed loop control, used when initializing device
*///////////////////////////////////////////////////////////////////
#define ENCODER          0x00
#define POTENTIOMETER    0x01
#define INV_ENCODER      0x02 // Inverting encoder
#define QUAD_ENCODER     0x03
#define NONE             0xff // I see no real reason to keep this here...

/*//////////////////////////////////////////////////////
Defines for mode in the id/mode array for enableing mode, makes more verbose and easier to see what is going on
*///////////////////////////////////////////////////////
#define VMODE 1
#define VCMODE 2
#define IMODE 3
#define SMODE 4
#define PMODE 5

/* //////////////////////////////////////////////////////////////////////
Macros defined for user input 
*////////////////////////////////////////////////////////////////////
#define GREATERTHAN 0x01
#define LESSTHAN 0x00
#define JUMPER 0x00
#define BRAKE 0x01
#define COAST 0x02

/*/////////////////////////////////////////////////////////////////////
values returned when querying the status of the limit switches
*//////////////////////////////////////////////////////////////////////
#define LIMIT_FWD     0x01
#define LIMIT_REV     0x02
#define LIMIT_S0FT_FWD    0x04
#define LIMIT_SOFT_REV    0x08

class CanJaguar{
private:
float power;
int integer;
float decimal;
int mode[16];

public:
CanJaguar(void);
float GetSpeed(int);
float Getposition(int);
void GetLimit(int,boolean,boolean,boolean,boolean);
int id;
void SetPower(int,float);
void SetVcomp(int,float);
void SetCurrent(int,float);
void SetSpeed(int,float);
void Setposition(int,float,float);
void SendConstants(int,int,float,float,float);
void ConfigPIDsensor(int,int,int,float);
void SetMaxVout(int,float);
void SetVramp(int,int);
void SetVcompRamps(int,int,int);
void ConfigSoftLimitForward(int,int,float);
void ConfigSoftLimitReverse(int,int,float);
 void EnableSoftLimit(int);
void DisableSoftLimit(int);
 void SetBrakeCoast(int,int);
void Initialize(void);
};

#endif

