/* *******************************************************************
 *      EXAMPLE SOFTWARE FOR USAGE WITH fsmlib.h
 * 
 *      2018, Lars Heinrichs
 *******************************************************************/

#include "fsmlib.h"
#include "stdio.h"

/* *******************************************************************
 *      S T A T E   M A C H I N E   D E S C R I P T I O N
 * 
 *      this section is used to describe the state machine. Each state
 *      machine is identified by its name.
 *      1 List states under FSM_STATES(name, ...). Default: first state given.
 *      2 List events under FSM_EVENTS(name, ...).
 *      3 Describe Transitions in FSM_TABLE(name, ...)
 *        Transitions are given as triplets {STATE, EVENT, FUNCTION}
 *        You may use FSM_STATE_TYPE(name) to get the state variable type
 *        for your transition functions. These functions must return
 *        a state.
 * ******************************************************************/

FSM_STATES(ledHandler,
    LED_IS_ON,
    LED_IS_OFF,
    LED_BLINK
)

FSM_EVENTS(ledHandler,
    LED_TURN_ON,
    LED_TURN_OFF,
    LED_SET_TOGGLE,
    LED_TIMER_TICK
)

static FSM_STATE_TYPE(ledHandler) led_turn_off(void* par, void* ret);
static FSM_STATE_TYPE(ledHandler) led_turn_on(void* par, void* ret);
static FSM_STATE_TYPE(ledHandler) led_toggle_mode_set(void* par, void* ret);
static FSM_STATE_TYPE(ledHandler) led_toggle(void* par, void* ret);

FSM_TABLE(ledHandler,
    {LED_IS_ON , LED_TURN_OFF,      led_turn_off        },
    {LED_IS_OFF, LED_TURN_ON ,      led_turn_on         },
    {LED_IS_ON , LED_SET_TOGGLE,    led_toggle_mode_set },
    {LED_IS_OFF, LED_SET_TOGGLE,    led_toggle_mode_set },
    {LED_BLINK,  LED_TURN_ON ,      led_turn_on         },
    {LED_BLINK,  LED_TURN_OFF,      led_turn_off        },
    {LED_BLINK,  LED_TIMER_TICK,    led_toggle          }
)

/* *******************************************************************
 *      A P P L I C A T I O N   S P E C I F I C   C O D E 
 * 
 *      The most important aspect for your application is to generate
 *      events that may be given to the state machine. For this example
 *      events are entered by the user in a console environment.
 * ******************************************************************/

int led_on = 1;

FSM_EVENT_TYPE(ledHandler) fsm_get_event() {
    int event = getchar();    
    int c;
    while (c = getchar() != '\n' && c!= EOF){}    
    return (FSM_EVENT_TYPE(ledHandler))(event-'0');
}

void print_instructions(){
    printf("    Available events:\n\
    0 Turn On (when blinking or off)\n\
    1 Turn off (when blinking or on)\n\
    2 Set Toggle mode (when not in toggle mode)\n\
    3 Toggle Tick (Toggle LED when in toggle mode)\n\
    LED currently turned on.\n");
}

void print_fsm_error(int fsm_error_code){
    switch(fsm_error_code){
        case FSM_OK: 
        break;
        case FSM_INVALID_EVENT:
            printf("invalid event\n");
        break;
        case FSM_INVALID_STATE:
            printf("invalid state\n");
        break;
        case FSM_NO_TRANSITION:
            printf("nothing done\n");
        break;
        default:
            printf("invalid error code returned\n");
        break;
        
    }
}

/* *******************************************************************
 *      M A I N   R O U T I N E
 * 
 *      To run the state machine, call fsm_run(name, event, par,
 *      ret). 'par' is meant to pass parameters to your state 
 *      transition functions and 'ret' to return information. In case
 *      the combination of given event and the machines current state
 *      is described by FSM_TABLE, the function defined for this 
 *      combination will be called. The functions must be implemented
 *      by the user and return FSM_STATES for the state machine.
 *       
 * ******************************************************************/

int peter(void){
    print_instructions();
    while(1){
        print_fsm_error(  
            fsm_run(ledHandler, 
                    fsm_get_event(), 
                    NULL, NULL)
        );
    }
}

/* *******************************************************************
 *      S T A T E   T R A N S I T I O N   F U N C T I O N S
 * 
 *      These functions are called based on the description in 
 *      FSM_TABLE. They return a state. For this simple example, the 
 *      parameter and return values are not used. You may pass a
 *      pointer and recast it to a non-void type as needed.
 * ******************************************************************/

static FSM_STATE_TYPE(ledHandler) led_turn_off(void* par, void* ret){
    printf("Static: LED off\n");
    led_on = 0;
    return LED_IS_OFF;
}

static FSM_STATE_TYPE(ledHandler) led_turn_on(void* par, void* ret){
    printf("Static: LED on\n");
    led_on = 1;
    return LED_IS_ON;
}

static FSM_STATE_TYPE(ledHandler) led_toggle_mode_set(void* par, void* ret){
    printf("Switched to Toggle Mode\n"); 
    return LED_BLINK;
}

static FSM_STATE_TYPE(ledHandler) led_toggle(void* par, void* ret){
    led_on = 1 - led_on;
    if (led_on)
        printf("Toggled: LED on\n");
    else 
        printf("Toggled: LED off\n");
    return LED_BLINK;
}
// EOF File led_handler_FSM.c
