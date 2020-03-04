#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework3.h"
#include "myGPIO.h"
#include "myTimer.h"


int main(void)
{
    // Count variables to control the LEDs.
    unsigned int count0 = 0;
    unsigned int count1 = 0;

    // TODO: Declare the variables that main uses to interact with your state machine.
    unsigned char buttonhistory = 0;

    // Stops the Watchdog timer.
    initBoard();
    // Initialize the GPIO.
    // YOU MUST WRITE THIS FUNCTION IN myGPIO.c
    initGPIO();
    // Initialize Timer0 to provide a one second count interval for updating LED2.
    // YOU MUST WRITE THIS FUNCTION IN myTimer.c
    initTimer(TIMER32_0_BASE, TIMER0_PRESCALER, TIMER0_COUNT);
    // Initialize Timer1 to provide a one millisecond count interval for updating the button history.
    // YOU MUST WRITE THIS FUNCTION IN myTimer.c
    initTimer(TIMER32_1_BASE, TIMER1_PRESCALER, TIMER1_COUNT);

    while(1)
    {
        // Update the color of LED2 using count0 as the index.
        // YOU MUST WRITE THIS FUNCTION BELOW.
        changeLaunchpadLED2(count0);
        // Update the color of the Boosterpack LED using count1 as the index.
        // YOU MUST WRITE THIS FUNCTION BELOW.

        changeBoosterpackLED(count1);

        // TODO: If Timer0 has expired, increment count0.
        // YOU MUST WRITE timer0expired IN myTimer.c

        if(timer0Expired())
        {
           count0++;
        }


        // TODO: If Timer1 has expired, update the button history from the pushbutton value.
        // YOU MUST WRITE timer1expired IN myTimer.c
        if(timer1Expired())
        {
           if(!checkStatus_BoosterpackS1()) //check pull up resistor button value
            {
                if(buttonhistory < 2) //increment buttonhistory up to 2 for constant hold of button
                    buttonhistory++;
            }
            else //if button is not constantly held, reset to 0
                buttonhistory = 0;


        }


        // TODO: Call the button state machine function to check for a completed, debounced button press.
        // YOU MUST WRITE THIS FUNCTION BELOW.
        if(fsmBoosterpackButtonS1(buttonhistory)) // on true press, increment counter
            count1++;


    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: Map the value of a count variable to a color for LED2.
// Since count is an unsigned integer, you can mask the value in some way.
void changeLaunchpadLED2(unsigned int count)
{
    unsigned int a = count%8;
switch(a)
{
    case 0:
        turnOff_LaunchpadLED2Red();
        turnOff_LaunchpadLED2Blue();
        turnOff_LaunchpadLED2Green();
        turnOff_LaunchpadLED1();
        break;
    case 1:
        turnOn_LaunchpadLED2Red();
        turnOff_LaunchpadLED2Blue();
        turnOff_LaunchpadLED2Green();
        turnOff_LaunchpadLED1();
        break;
    case 2:
        turnOff_LaunchpadLED2Red();
        turnOff_LaunchpadLED2Blue();
        turnOn_LaunchpadLED2Green();
        turnOff_LaunchpadLED1();
        break;
    case 3:
        turnOn_LaunchpadLED2Red();
        turnOn_LaunchpadLED2Green();
        turnOff_LaunchpadLED2Blue();
        turnOff_LaunchpadLED1();
        break;
    case 4:
        turnOff_LaunchpadLED2Red();
        turnOff_LaunchpadLED2Green();
        turnOn_LaunchpadLED2Blue();
        turnOff_LaunchpadLED1();
        break;
    case 5:
        turnOn_LaunchpadLED2Red();
        turnOff_LaunchpadLED2Green();
        turnOn_LaunchpadLED2Blue();
        turnOff_LaunchpadLED1();
        break;
    case 6:
        turnOff_LaunchpadLED2Red();
        turnOn_LaunchpadLED2Green();
        turnOn_LaunchpadLED2Blue();
        turnOff_LaunchpadLED1();
        break;
    case 7:
        turnOn_LaunchpadLED2Red();
        turnOn_LaunchpadLED2Green();
        turnOn_LaunchpadLED2Blue();
        turnOff_LaunchpadLED1();
        break;
    default:
        turnOn_LaunchpadLED1();
        turnOff_LaunchpadLED2Red();
        turnOff_LaunchpadLED2Blue();
        turnOff_LaunchpadLED2Green();
        break;
}


}

// TODO: Maybe the value of a count variable to a color for the Boosterpack LED
// This is essentially a copy of the previous function, using a different LED
void changeBoosterpackLED(unsigned int count)
{
    unsigned int a = count%8;
switch(a)
{
    case 0:
        turnOff_BoosterpackLEDRed();
        turnOff_BoosterpackLEDBlue();
        turnOff_BoosterpackLEDGreen();
        break;
    case 1:
        turnOn_BoosterpackLEDRed();
        turnOff_BoosterpackLEDBlue();
        turnOff_BoosterpackLEDGreen();
        break;
    case 2:
        turnOff_BoosterpackLEDRed();
        turnOff_BoosterpackLEDBlue();
        turnOn_BoosterpackLEDGreen();
        break;
    case 3:
        turnOn_BoosterpackLEDRed();
        turnOff_BoosterpackLEDBlue();
        turnOn_BoosterpackLEDGreen();
        break;
    case 4:
        turnOff_BoosterpackLEDRed();
        turnOn_BoosterpackLEDBlue();
        turnOff_BoosterpackLEDGreen();
        break;
    case 5:
        turnOn_BoosterpackLEDRed();
        turnOn_BoosterpackLEDBlue();
        turnOff_BoosterpackLEDGreen();
        break;
    case 6:
        turnOff_BoosterpackLEDRed();
        turnOn_BoosterpackLEDBlue();
        turnOn_BoosterpackLEDGreen();
        break;
    case 7:
        turnOn_BoosterpackLEDRed();
        turnOn_BoosterpackLEDBlue();
        turnOn_BoosterpackLEDGreen();
        break;
    default:
        turnOn_LaunchpadLED1();
        turnOff_BoosterpackLEDRed();
        turnOff_BoosterpackLEDBlue();
        turnOff_BoosterpackLEDGreen();
        break;
}
}

// TODO: Create a button state machine.
// The button state machine should return true or false to indicate a completed, debounced button press.
typedef enum {UP, DOWN} BUTTONSTATE;
bool fsmBoosterpackButtonS1(unsigned char buttonhistory)
{
    static BUTTONSTATE currentstate = UP;//starts in state up/unpressed

    bool pressed = false; //pressed is passed back, always initialize as false
switch(currentstate)
    {
case UP: //if button history has been held for timer1 215 cycles, set state to down and return a true press
    if(buttonhistory == 2)
    {
        currentstate = DOWN;
        pressed = true;
    }
    break;
case DOWN:
    if(!buttonhistory) //if button history has not been held long enough return to up state and wait for true press
        currentstate = UP;
    break;
    }

    return pressed;
}
