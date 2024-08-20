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
#ifndef __BUTTONS_H
#define __BUTTONS_H

/**
 * @brief   Button control structure
 */
typedef struct _ButtonType
{
    uint8_t Pin;      /*!< Pin where the button is connected*/
    uint8_t Port;     /*!< Port where the pin is connected if applicable*/
    uint8_t Counter;  /*!< Counter for the number of samples required*/
    uint8_t Event;    /*!< store the event, PRESS, RELEASE or IDLE*/
    uint8_t Status;   /*!< Button status, ACTIVE, INACTIVE*/
    uint8_t ActiveLevel; /*!< Active level of the button*/
    uint8_t smState;  /*!< Internal State machine for the button*/
} ButtonType;

/**
 * @brief   Keyboard control structure  
 */
typedef struct _KeyboardType
{
    uint8_t Buttons;    /*!< the number of buttons to handle*/
    uint8_t Sample;     /*!< the number of samples necesesary to detect an activation*/
    uint8_t Counter;    /*!< counter for the nmber of bttons registered*/
    ButtonType *BtnBuffer; /*!< pointer to array of ButtonType*/
} KeyboardType;


/**
 * @brief   Button events
 */
typedef enum _BtnStatus
{
    BTN_IDLE = 0,   /*!< No event*/
    BTN_PRESSED,    /*!< button has been pressed*/
    BTN_RELEASED    /*!< button has been released*/
} BtnEvents;

/**
  * @defgroup Buttons_states Button states active (pressed) and inactive
  @{ */
#define BTN_INACTIVE  0 /*!< Button is inactive*/
#define BTN_ACTIVE    1   /*!< Button is active*/
/**
  @} */

void Buttons_Init( KeyboardType *Buttons, uint8_t NButtons, uint8_t Samples, ButtonType *Buffer );
uint8_t Buttons_Register( KeyboardType *Buttons, uint8_t Pin, uint8_t Port, uint8_t ActiveLevel );
uint8_t Buttons_GetStatus( KeyboardType *Buttons, uint8_t Button );
uint8_t Buttons_GetEvent( KeyboardType *Buttons, uint8_t Button );
void Buttons_MainFunction( KeyboardType *Buttons );

#endif
