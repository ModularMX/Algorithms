/**
 * @file buttons.h
 * @brief **Button handler manager**
 * 
 * This is a simple button handler manager that allows to handle multiple buttons with a single
 * periodic function. The buttons are sampled and the status is stored in a buffer. The user can
 * check the status of the buttons and the events that have been detected.
 * 
 * The code was written for the STM32xx family of microcontrollers, but it can be easily ported
 * to other microcontrollers.  
 */
#include "bsp.h"
#include "buttons.h"

/**
 * @brief   States of the button state machine
 */
typedef enum _ButtonStates
{
    ST_IDLE = 0,    /*!< Idle state*/
    ST_PRESS,       /*!< Press state*/
    ST_HOLD,        /*!< Hold state*/
    ST_RELEASE      /*!< Release state*/
} ButtonStates;

/**
 * @brief   Array of GPIO ports from the STM32G0xx family
 */
static GPIO_TypeDef *Ports[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF };

static void Sample_Button( ButtonType *Button, uint8_t Samples );

/**
 * @brief   Initialize the button handler
 * 
 * This function initializes the button handler with the number of buttons to handle, the number of samples
 * to detect an activation and the buffer to store the buttons structure.
 * 
 * @param Buttons Pointer to the KeyboardType structure
 * @param NButtons Number of buttons to handle
 * @param Samples Number of samples to detect an activation
 * @param Buffer Pointer to the buffer array to store the buttons structure
 * 
 * @return void
 */
void Buttons_Init( KeyboardType *Buttons, uint8_t NButtons, uint8_t Samples, ButtonType *Buffer )
{
    Buttons->Buttons = NButtons;
    Buttons->Sample = Samples;
    Buttons->BtnBuffer = Buffer;
    Buttons->Counter = 0;
}

/**
 * @brief   Register a button
 * 
 * This function registers a button in the button handler. The button is defined by the pin, port and the active level.
 * 
 * @param Buttons Pointer to the KeyboardType structure
 * @param Pin Pin where the button is connected
 * @param Port Port where the pin is connected
 * @param ActiveLevel Active level of the button
 * 
 * @return uint8_t The button number
 */
uint8_t Buttons_Register( KeyboardType *Buttons, uint8_t Pin, uint8_t Port, uint8_t ActiveLevel )
{
    uint8_t result = 0;

    if( Buttons->Counter < Buttons->Buttons )
    {
        Buttons->BtnBuffer[Buttons->Counter].Pin = Pin;
        Buttons->BtnBuffer[Buttons->Counter].Port = Port;
        Buttons->BtnBuffer[Buttons->Counter].ActiveLevel = ActiveLevel;
        Buttons->BtnBuffer[Buttons->Counter].Status = BTN_INACTIVE;
        Buttons->BtnBuffer[Buttons->Counter].Event = BTN_IDLE;
        Buttons->BtnBuffer[Buttons->Counter].smState = ST_IDLE;
        Buttons->Counter++;
        result = Buttons->Counter;
    }
       
    return result;
}

/**
 * @brief   Get the status of a button
 * 
 * This function returns the status of a button, ACTIVE or INACTIVE. been active means that 
 * the button is pressed otherwise is inactive.
 * 
 * @param Buttons Pointer to the KeyboardType structure 
 * @param Button Button number
 * 
 * @return uint8_t The status of the button
 */
uint8_t Buttons_GetStatus( KeyboardType *Buttons, uint8_t Button )
{
    uint8_t result = BTN_INACTIVE;

    if( Button < Buttons->Counter )
    {
        result = Buttons->BtnBuffer[Button - 1].Status;
    }

    return result;
}

/**
 * @brief   Get the event of a button
 * 
 * This function returns the event of a button, PRESSED or RELEASED. After this fucntion is called
 * the event is cleared.
 * 
 * @param Buttons Pointer to the KeyboardType structure
 * @param Button Button number
 * 
 * @return The event of the button
 */
uint8_t Buttons_GetEvent( KeyboardType *Buttons, uint8_t Button )
{
    uint8_t result = BTN_IDLE;

    if( Button <= Buttons->Counter )
    {
        result = Buttons->BtnBuffer[Button - 1].Event;
        Buttons->BtnBuffer[Button - 1].Event = BTN_IDLE;
    }
    
    return result;
}

/**
 * @brief   Main function for the button handler
 * 
 * This function is the main function for the button handler. It samples the buttons and stores the status
 * and events in the buffer.
 * 
 * @param Buttons Pointer to the KeyboardType structure
 */
void Buttons_MainFunction( KeyboardType *Buttons )
{
    for( uint8_t btn = 0; btn < Buttons->Counter; btn++ )
    {
        /*sample one single button*/
        Sample_Button( &Buttons->BtnBuffer[btn], Buttons->Sample );
    }
}

/**
 * @brief   Sample the button
 * 
 * This function samples the button and stores the status and events in the buffer.
 * 
 * @param Button Pointer to the ButtonType structure
 * @param Samples Number of samples to detect an activation
 * 
 * @return void
 */
static void Sample_Button( ButtonType *Button, uint8_t Samples )
{   
    switch( Button->smState )
    {
        /*ask if the button has been pressed*/
        case ST_IDLE:
            if( HAL_GPIO_ReadPin( Ports[Button->Port], (1<<Button->Pin) ) == Button->ActiveLevel )
            {
                Button->smState = ST_PRESS; /*change to press state*/
                Button->Counter = 0; /*start the counter*/
            }
        break;
        
        case ST_PRESS:
            /*increase counter*/
          Button->Counter++;
          /*Ask if the button was release after the timeout*/
          if( HAL_GPIO_ReadPin( Ports[Button->Port], (1<<Button->Pin) ) != Button->ActiveLevel )
          {
              /*it was a spurios detection, ignore and get back idle*/
              Button->smState = ST_IDLE;
          }
          /*ask if the number of samples has been pass*/
          if( Button->Counter >= Samples )
          {
              /*if so, an actual press action is detected*/
              Button->Event = BTN_PRESSED;
              Button->Status = BTN_ACTIVE;
              Button->smState = ST_HOLD;
          }
        break;
        /*as if the button has been released after been pressed*/
        case ST_HOLD:
            if( HAL_GPIO_ReadPin( Ports[Button->Port], (1<<Button->Pin) ) != Button->ActiveLevel )
            {
                Button->smState = ST_RELEASE;
                Button->Counter = 0; /*start the counter*/
            }
        break;
        case  ST_RELEASE:
            /*increase counter*/
            Button->Counter++;
            /*Ask if the button was release after the timeout*/
            if( HAL_GPIO_ReadPin( Ports[Button->Port], (1<<Button->Pin) ) == Button->ActiveLevel )
            {
                /*it was a spurios detection, ignore and get back release detection*/
                Button->smState = ST_HOLD;
            }
            /*ask if the number of samples has been pass*/
            if( Button->Counter >= Samples)
            {
                /*if so, an actual release action is detected*/
                Button->Event = BTN_RELEASED;
                Button->Status = BTN_INACTIVE;
                Button->smState = ST_IDLE;
            }
        break;
    }
}
