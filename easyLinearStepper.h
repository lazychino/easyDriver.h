/*
 * easyLinearStepper.h
 * 
 * a library for controlling linear stepper actuators
 *  
 * Author: Pedro A. Melendez
 * email: lazy.chino@gmail.com
 *
 */
 
#ifndef EasyLinearStepper_H
#define EasyLinearStepper_H

#include "Arduino.h"

#define OPEN 1
#define CLOSE 0

#define FORWARD 1
#define BACKWARD 0

class linearStepper
{
private:
    // arduino pins for controlling
    uint8_t direction;
    uint8_t enable;
    uint8_t step;
    uint8_t o_limit;
    uint8_t c_limit;
    long double travel_step;
    
    
public:
    linearStepper(){}
    ~linearStepper(){}
    void attach(uint8_t, uint8_t, uint8_t, long double); 
    void attach(uint8_t, uint8_t, uint8_t, long double, uint8_t); // with zero limit switch
    
    void move(const bool, double);
    void moveLimit(const bool, const double);
    
    
    void moveSlow(const bool, double);
    void moveConst2Limit(const bool);
};

#endif
