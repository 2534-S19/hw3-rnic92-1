/*
 * myTimer.c
 *
 *  Created on: 04Mar20
 *      Author: Nicolas Rohr
 */
#define UINTMAX 65536
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "myTimer.h"

// TODO: Write a function that can intialize a Timer with particular values for pre-scale and count.
// Aliases for the Timers and the preScaler arguments are defined in myTimer.h
void initTimer(unsigned int timer, unsigned int preScaler, unsigned int count)
{

    Timer32_initModule(timer, preScaler, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_setCount(timer,count);
    Timer32_startTimer(timer, REPEAT);

}

// TODO: Write a function that indicates whether Timer0 has expired.
// You have been given a such a function in the lecture slides.
bool timer0Expired(void)
{
    static unsigned int previousSnap = UINTMAX;
    unsigned int currentSnap;
    bool returnValue;

    currentSnap = Timer32_getValue(TIMER32_0_BASE);
    returnValue = currentSnap > previousSnap;
    previousSnap = currentSnap;
    return returnValue;
}

// TODO: Write a function that indicates whether Timer1 has expired.
// This is essentially a copy of the previous function, using a different timer.
// Since we only have two timers, later in the course we will create software timers that use a common time base.
bool timer1Expired(void)
{
    static unsigned int previousSnap2 = UINTMAX;
    unsigned int currentSnap;
    bool returnValue;

    currentSnap = Timer32_getValue(TIMER32_1_BASE);
    returnValue = currentSnap > previousSnap2;
    previousSnap2 = currentSnap;
    return returnValue;
}
