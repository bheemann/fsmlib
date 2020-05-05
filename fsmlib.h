/********************************************************************************
 * Copyright (c) <2018>                                                         *
 * Benjamin Heemann, Alexander Kann, Lars Heinrichs                             *
 * Version 1.0                                                                  *
 *                                                                              *
 * Permission is hereby granted, free of charge, to any person obtaining a copy *
 * of this software and associated documentation files (the "Software"), to     *
 * deal in the Software without restriction, including without limitation the   *
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or  *
 * sell copies of the Software, and to permit persons to whom the Software is   *
 * furnished to do so, subject to the following conditions:                     *
 *                                                                              *
 * The above copyright notice and this permission notice shall be included in   *
 * all copies or substantial portions of the Software.                          *
 *                                                                              *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE  *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER       *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,*
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN    *
 * THE SOFTWARE.                                                                *
 *******************************************************************************/

#ifndef FSM_LIB_HEADER
#define FSM_LIB_HEADER

#ifdef __cplusplus
extern "C"
{
#endif
/*******************************************************************************
 *    INCLUDED FILES
 ******************************************************************************/
#include "stdint.h"

/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/
#define fsm_run(name, event, param, ret) \
name##_fsm_step(event, param, ret)

#define FSM_STATE_TYPE(name) \
name##_states_t

#define FSM_EVENT_TYPE(name) \
name##_events_t

/*******************************************************************************
 *    STATE MACHINE RETURN VALUES
 ******************************************************************************/
typedef enum fsmReturn_t{
    FSM_INVALID_STATE   = -1,
    FSM_INVALID_EVENT   = -2,
    FSM_NO_TRANSITION   = -3,
    FSM_NULL_EXCEPTION  = -4,
    FSM_OK              = 0    
}fsmReturn_t;

/*******************************************************************************
 *    PUBLIC TYPES
 ******************************************************************************/
#define FSM_STATES(name, ...)               \
typedef enum name##_states_t{               \
    __VA_ARGS__,                            \
    name##_STATE_COUNT                      \
} name##_states_t;                          \
                                            \
typedef struct name##_fsm_t {               \
    uint8_t state;                          \
    uint8_t event;                          \
    name##_states_t (*func)(void*, void*);  \
} name##_fsm_t;


#define FSM_EVENTS(name, ...)               \
typedef enum name##_events_t{               \
    __VA_ARGS__,                            \
    name##_EVENT_COUNT                      \
} name##_events_t;                          \
                                            \
fsmReturn_t name##_fsm_step(name##_events_t event, void* param, void* ret);

/*******************************************************************************
 *    STATE MACHINE CORE LOGIC
 ******************************************************************************/
#define FSM_TABLE(name, ...)                                \
name##_fsm_t name##_fsm[] = {                               \
    __VA_ARGS__                                             \
};                                                          \
                                                            \
static name##_states_t        name##_state = (name##_states_t)0;             \
static name##_fsm_t         * name##_transition;            \
                                                            \
fsmReturn_t name##_fsm_step(name##_events_t event, void* param, void* ret){ \
    if (event >= name##_EVENT_COUNT)                        \
        return FSM_INVALID_EVENT;                           \
                                                            \
    if (name##_state >= name##_STATE_COUNT )                \
        return FSM_INVALID_STATE;                           \
                                                            \
    name##_transition = name##_fsm;                         \
    for (uint8_t i = 0;                                     \
        i < sizeof(name##_fsm) / sizeof(name##_fsm_t);      \
        i++, name##_transition++)                           \
        {                                                   \
        if (name##_transition->state == name##_state &&     \
            name##_transition->event == event)              \
        {                                                   \
            if (name##_transition->func == NULL)            \
                return FSM_NULL_EXCEPTION;                  \
            name##_state = name##_transition->func(param, ret);\
            return FSM_OK;                                  \
        }                                                   \
    }                                                       \
    return FSM_NO_TRANSITION;                               \
}

#ifdef __cplusplus
}
#endif
#endif // FSM_LIB_HEADER
