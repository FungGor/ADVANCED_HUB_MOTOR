/*
 * ESCOOTER_BrakeAndThrottle.h
 *
 *  Created on: 7 Jul 2023
 *      Author: TerenceLeung
 */

#ifndef ESCOOTER_ESCOOTER_BRAKEANDTHROTTLE_H_
#define ESCOOTER_ESCOOTER_BRAKEANDTHROTTLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "ESCOOTER_MainTask.h"

typedef struct
{
    int16_t  TARGET_IQ;
    int16_t  IQ_LIMIT;
    int16_t  SPEED_LIMIT;
    uint16_t  RAMP_DURATION;
    bool  brake;
}ESCOOTER_BrakeANDThrottleInput_t;


/**To be called by motor control protocol**/
/**Before running the Main Task, please verify this function carefully**/
/*Input some dummy commands -> check it*/
bool ESCOOTER_ThrottleSignalTrigger(ESCOOTER_BrakeANDThrottleInput_t *inputHandle);


/**To be called by motor control protocol**/
/**Before running the Main Task, please verify this function carefully**/
/*Input some dummy commands -> check it*/
bool ESCOOTER_BrakeSignalTrigger(ESCOOTER_Operation_t *opHandle, ESCOOTER_BrakeANDThrottleInput_t *inputHandle);

/**To be called by motor control protocol**/
/**Before running the Main Task, please verify this function carefully**/
/*Input some dummy commands -> check it*/
void ESCOOTER_DEBUG_ReleaseBrakeSignal(ESCOOTER_Operation_t *opHandle, ESCOOTER_BrakeANDThrottleInput_t *inputHandle);

#ifdef __cplusplus
}
#endif
#endif /* ESCOOTER_ESCOOTER_BRAKEANDTHROTTLE_H_ */
