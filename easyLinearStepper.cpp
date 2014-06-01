/*
 * easyLinearStepper.cpp
 * 
 * a library for controlling linear stepper actuators using stepper driver
 * 
 *  
 * Author: Pedro A. Melendez
 * email: lazy.chino@gmail.com
 *
 */

#include "Arduino.h"
#include "easyLinearStepper.h"


inline void linearStepper::oneStep()
{
    digitalWrite(step, HIGH);
    delayMicroseconds(1);
    digitalWrite(step, LOW);
    delayMicroseconds(period);
}


void linearStepper::accel()
{
    if(period > minPeriod)
        period -= acceleration;
}


void linearStepper::decel()
{
    if(period < maxPeriod)
        period += acceleration;
}

void linearStepper::setAccel(unsigned int accelConst)
{
    acceleration = accelConst;
}

void linearStepper::setLimit1(unsigned short limitPin)
{
    limit1 = limitPin;
}


void linearStepper::setLimit2(unsigned short limitPin)
{
    limit2 = limitPin;    
}

/* spd2period
 * 
 * takes speed on in/s or mm/s and calculate the corresponding period
 * the speed unit depends on the unit for travel per step
 * use the SAME UNIT always
 */
unsigned int linearStepper::spd2period(double spd)
{
    return (travelPerStep/spd)*1000;
}


void linearStepper::setMinSpeed(double spd)
{
    maxPeriod = spd2period(spd);
}

void linearStepper::setMaxSpeed(double spd)
{
    minPeriod = spd2period(spd);
}


/*
 * dir: direction pin 
 * stp: step pin
 * ena: enabled motor pin
 * distancePerStep: linear distance traveled for each step of the motor
 */
void linearStepper::attach( 
                            unsigned short dir, 
                            unsigned short stp, 
                            unsigned short ena, 
                            double distancePerStep
                        )
{
    direction = dir;
    step = stp;
    enable = ena;
    travelPerStep = distancePerStep;
    
    pinMode(dir, OUTPUT);
    pinMode(stp, OUTPUT);
    pinMode(ena, OUTPUT);
    digitalWrite(ena, HIGH);
}



/**
 * move with acelaration
 * 
 * dir: {0, 1}
 * dist: distance to move the linear stepper actuator (inches)
 */
void linearStepper::move(const bool dir, double dist)
{    
    uint32_t steps = dist/travelPerStep;
    
    digitalWrite(direction, dir);  
    
    period = minPeriod;            // set initial speed
    digitalWrite(enable, LOW);      // enable board
    //~ Serial.println(steps);
    
    uint32_t end = steps*0.25
    uint32_t begin = steps - end
    
    for(uint32_t i=steps; i > 0; i--)
    {
        oneStep();
            
        if( i >= begin)
        { 
            accel();
        }
        else if( i < end)
        { 
            decel();
        }
        else{}
    }
    digitalWrite(enable, HIGH);         // disable board
}

void linearStepper::moveLimit(const bool dir, double dist)
{    
    uint32_t steps = dist/travelPerStep;
    
    digitalWrite(direction, dir);  
    
    period = minPeriod;             // set initial speed
    digitalWrite(enable, LOW);      // enable board
    
    for(uint32_t i=steps; i > 0; i--)
    {
        oneStep();
        
        if(digitalRead(limit2) == 0)
            break;
            
        if( i >= begin)
        { 
            accel();
        }
        else if( i < end)
        { 
            decel();
        }
        else{}
    }
    int done = 0;
    while(1)
    {
        oneStep();
            
        if(digitalRead(limit2) == 0)
            done++;
        if(done == 32000)    
            break;
    }
    digitalWrite(enable, HIGH);         // disable board
}

void linearStepper::moveConst2Limit(const bool dir)
{    
    
    digitalWrite(direction, dir);  
    
    period = (minPeriod+maxPeriod)/2;   // set speed
    digitalWrite(enable, LOW);      // enable board
    
    while(true)
    {
        oneStep();
            
        if(digitalRead(limit2) == 0)
            break;
    }
    digitalWrite(enable, HIGH);         // disable board
}

/**
 * move with constant slow speep
 * 
 * dir: {0, 1}
 * steps: quatity of microsteps to move, 3200 steps = 1 revolution
 */
void linearStepper::moveConst(const bool dir, double dist)
{
    uint32_t steps = dist/travelPerStep;

    digitalWrite(direction, dir);  // set direction
    period = (minPeriod+maxPeriod)/2;   // set speed
    
    digitalWrite(enable, LOW);      // enable board
    //~ Serial.println(steps);
    for(uint32_t i=steps; i > 0; i--)
    {
        oneStep();
        //~ Serial.println("step");
    }
    digitalWrite(enable, HIGH);         // disable board
}

