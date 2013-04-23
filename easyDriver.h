/*
 * easyDriver.h
 * 
 * a library for controlling a single stepper motor using 
 * the Easy Driver www.schmalzhaus.com/EasyDriver
 *  
 * 
 * Author: Pedro A. Melendez
 * email: lazy.chino@gmail.com
 *
 */


#include <Arduino.h>

class easyDriver
{
private:
    // arduino pins for controlling
    unsigned short Direction;
    unsigned short enable;
    unsigned short Step;
    
public:
    easyDriver(){}
    ~easyDriver(){}
    void attach(unsigned short, unsigned short, unsigned short);
    void move(const char, const unsigned long);
};

void easyDriver::attach(unsigned short dir, unsigned short stp, unsigned short ena)
{
    Direction = dir;
    Step = stp;
    enable = ena;
    
    pinMode(dir, OUTPUT);
    pinMode(stp, OUTPUT);
    pinMode(ena, OUTPUT);
    digitalWrite(ena, HIGH);
}

void easyDriver::move(const char dir, const unsigned long steps)
{    
    
    if( dir == 'O')
    {
	digitalWrite(Direction, HIGH);	// set direction outward
    }
    else if(dir == 'C')
    {
	digitalWrite(Direction, LOW);	// set direction inward
    }
    else{ return; }
    
    int period = 85;				// set initial speed
    digitalWrite(enable, LOW);		// enable board
    
    for(unsigned long i=steps; i > 0; i--)
    {
	delayMicroseconds(period);			//one square pulse is send to the driver
	digitalWrite(Step, HIGH);
	delayMicroseconds(period);
	digitalWrite(Step, LOW);
        
	if( period > 18 && i%400 == 0)		// on the first revolutions
		period--;			// the period of the pulse is decrease to accelerate

	else if( i < 3200 && i%20 == 0)		// on the last revolution period
		period++;				// increase to decelerate
	else{}
    }
    digitalWrite(enable, HIGH);			// disable board
}
