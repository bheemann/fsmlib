# fsmlib.h

This library is another approach to implement a finite state machine in C. 

Implementation should be:

 * easy
 * well manageable
 * well readable
 * standardized

## Getting Started

Include the header:
```
#include "fsmlib.h"
```

Define states:
```
FSM_STATES(ledHandler,
    sLED_IS_ON,
    sLED_IS_OFF,
    sLED_BLINK
)
```

Define events:
```
FSM_EVENTS(ledHandler,
    eLED_TURN_ON,
    eLED_TURN_OFF,
    eLED_SET_TOGGLE,
    eLED_TIMER_TICK
)
```

Define callback functions:
```
static FSM_STATE_TYPE(ledHandler) led_turn_off(void* par, void* ret);
static FSM_STATE_TYPE(ledHandler) led_turn_on(void* par, void* ret);
static FSM_STATE_TYPE(ledHandler) led_toggle_mode_set(void* par, void* ret);
static FSM_STATE_TYPE(ledHandler) led_toggle(void* par, void* ret);
```

Implement the FSM_TABLE as a triplet of {STATE, EVENT, FUNCTION}
```
FSM_TABLE(ledHandler,
    {sLED_IS_ON , eLED_TURN_OFF,      led_turn_off        },
    {sLED_IS_OFF, eLED_TURN_ON ,      led_turn_on         },
    {sLED_IS_ON , eLED_SET_TOGGLE,    led_toggle_mode_set },
    {sLED_IS_OFF, eLED_SET_TOGGLE,    led_toggle_mode_set },
    {sLED_BLINK,  eLED_TURN_ON ,      led_turn_on         },
    {sLED_BLINK,  eLED_TURN_OFF,      led_turn_off         },
    {sLED_BLINK,  eLED_TIMER_TICK,    led_toggle          }
)
```

The most important aspect for your application is to generate events that may be given to the state machine. For this example events are entered by the user in a console environment.
```
int fsm_get_event() {
    int event = getchar();    
    int c;
    while (c = getchar() != '\n' && c!= EOF){}    
    return (event-'0');
}
```

To run the state machine, call fsm_run(name, event, par, ret). 'par' is meant to pass parameters to your state transition functions and 'ret' to return information. In case the combination of given event and the machines current state is described by FSM_TABLE, the function defined for this combination will be called. The functions must be implemented by the user and return FSM_STATES for the state machine.

```
int main(void){
    print_instructions();
    while(1){
        print_fsm_error(  
            fsm_run(ledHandler, 
                    fsm_get_event(), 
                    NULL, NULL)
        );
    }
}
```

For a complete example look at [fsmlib_example.c](fsmlib_example.c). Just compile it using gcc:

```
gcc fsmlib_example.c
```

HAVE FUN!

## Tests

It is planned to add unit tests using ceedling 

## Contributing

Please contact me if you have any suggestions for improvements.

## Versioning

The versionnumber gets updated in the License File.

## License

This project is licensed under the MIT License - see the [LICENSE.txt](LICENSE.txt) file for details

## Acknowledgment

Thanks to Alexander Kann and Lars Heinrichs for their valuable support. 
