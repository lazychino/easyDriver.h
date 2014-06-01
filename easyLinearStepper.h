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

class linearStepper
{
private:
    // arduino pins for controlling
    unsigned short direction;
    unsigned short enable;
    unsigned short step;
    unsigned short limit1;
    unsigned short limit2;
    
    // variables
    double travelPerStep;
    unsigned int period;             // period between step in microseconds
    unsigned int acceleration;
    unsigned int minPeriod;
    unsigned int maxPeriod;
    
    inline void oneStep();
    void accel();
    void decel();
    unsigned int spd2period(double);
    
public:
    linearStepper(){}
    ~linearStepper(){}
    
    void attach(unsigned short, unsigned short, unsigned short, double); 
    
    void move(const bool, double);
    void moveLimit(const bool, double);
    void moveConst(const bool, double);
    void moveConst2Limit(const bool);
    
    void setAccel(unsigned int);
    void setLimit1(unsigned short);
    void setLimit2(unsigned short);
    void setMinSpeed(double);
    void setMaxSpeed(double);
};

#endif
