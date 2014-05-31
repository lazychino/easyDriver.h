/*
 * easyLinearStepper.cpp
 * 
 * a library for controlling linear stepper actuators
 *  
 * Author: Pedro A. Melendez
 * email: lazy.chino@gmail.com
 *
 */

#include "Arduino.h"
#include "easyLinearStepper.h"


void linearStepper::attach(uint8_t dir, uint8_t stp, uint8_t ena, long double distancePerStep)
{
    direction = dir;
    step = stp;
    enable = ena;
    travel_step = distancePerStep;
    
    pinMode(dir, OUTPUT);
    pinMode(stp, OUTPUT);
    pinMode(ena, OUTPUT);
    digitalWrite(ena, HIGH);
}

void linearStepper::attach(uint8_t dir, uint8_t stp, uint8_t ena, long double distancePerStep, uint8_t limitClose)
{
    direction = dir;
    step = stp;
    enable = ena;
    c_limit = limitClose;
    travel_step = distancePerStep;
    
    pinMode(dir, OUTPUT);
    pinMode(stp, OUTPUT);
    pinMode(ena, OUTPUT);
    pinMode(limitClose, INPUT_PULLUP);
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
    uint32_t steps = (dist/travel_step)*16;
    
    if(dir)
    {
        digitalWrite(direction, HIGH);  // set direction +
    }
    else
    {
        digitalWrite(direction, LOW);   // set direction -
    }
    
    int period = 75;            // set initial speed
    digitalWrite(enable, LOW);      // enable board
    //~ Serial.println(steps);
    
    for(uint32_t i=steps; i > 0; i--)
    {
        delayMicroseconds(period);      //one square pulse is send to the driver equals one step
        digitalWrite(step, HIGH);
        delayMicroseconds(period);
        digitalWrite(step, LOW);
            
        if( i >= 3200)
        { 
            if(period > 18 && i%400 == 0)       // on the first revolutions
            period--;           // the period of the pulse is decrease to accelerate
        }
        else if( i < 3200)
        { 
            if( i%20 == 0)      // on the last revolution period
            period++;               // increase to decelerate
        }
        else{}
    }
    digitalWrite(enable, HIGH);         // disable board
}

void linearStepper::moveLimit(const bool dir, const double dist)
{    
    uint32_t steps = (dist/travel_step)*16;
    
    if(dir)
    {
    digitalWrite(direction, HIGH);  // set direction outward
    }
    else
    {
    digitalWrite(direction, LOW);   // set direction inward
    }
    
    int period = 75;            // set initial speed
    digitalWrite(enable, LOW);      // enable board
    
    for(uint32_t i=steps; i > 0; i--)
    {
        delayMicroseconds(period);      //one square pulse is send to the driver equals one step
        digitalWrite(step, HIGH);
        delayMicroseconds(period);
        digitalWrite(step, LOW);
        
        if(digitalRead(c_limit) == 0)
            break;
            
        if( i >= 3200)
        { 
            if(period > 18 && i%400 == 0)       // on the first revolutions
            period--;           // the period of the pulse is decrease to accelerate
        }
        else if( i < 3200)
        { 
            if(period < 75 && i%20 == 0)        // on the last revolution period
            period++;               // increase to decelerate
        }
        else{}
    }
    int done = 0;
    while(1)
    {
        delayMicroseconds(period);      //one square pulse is send to the driver equals one step
        digitalWrite(step, HIGH);
        delayMicroseconds(period);
        digitalWrite(step, LOW);
            
            if(digitalRead(c_limit) == 0)
            done++;
        if(done == 32000)    
            break;
    }
    digitalWrite(enable, HIGH);         // disable board
}

void linearStepper::moveConst2Limit(const bool dir)
{    
    
    if(dir)
    {
    digitalWrite(direction, HIGH);  // set direction outward
    }
    else
    {
    digitalWrite(direction, LOW);   // set direction inward
    }
    
    int period = 50;            // set initial speed
    digitalWrite(enable, LOW);      // enable board
    
    for(uint32_t i=1000000; 1; i--)
    {
    delayMicroseconds(period);      //one square pulse is send to the driver equals one step
    digitalWrite(step, HIGH);
    delayMicroseconds(period);
    digitalWrite(step, LOW);
        
    if(digitalRead(c_limit) == 0)
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
void linearStepper::moveSlow(const bool dir, double dist)
{
    uint32_t steps = (dist/travel_step)*16;
    moveSlow(dir, steps);  
    
    if(dir)
    {
    digitalWrite(direction, HIGH);  // set direction outward
    }
    else
    {
    digitalWrite(direction, LOW);   // set direction inward
    }
    
    int period = 50;            // set initial speed
    digitalWrite(enable, LOW);      // enable board
    //~ Serial.println(steps);
    for(uint32_t i=steps; i > 0; i--)
    {
    delayMicroseconds(period);      //one square pulse is send to the driver equals one step
    digitalWrite(step, HIGH);
    delayMicroseconds(period);
    digitalWrite(step, LOW);
    //~ Serial.println("step");
    }
    digitalWrite(enable, HIGH);         // disable board
}

