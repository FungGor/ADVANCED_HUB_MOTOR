/*
 * SafetySetting.h
 *
 *  Created on: 6 Oct 2023
 *      Author: TerenceLeung
 */

#ifndef ESCOOTER_SAFETYSETTING_H_
#define ESCOOTER_SAFETYSETTING_H_

#include "Cruise_Control.h"

#define PROPORTIONAL_GAIN    1000  /*Scaled*/
#define INTEGRAL_GAIN        2000  /*Scaled*/
#define DIFFERENTIAL_GAIN    3000  /*Scaled*/

extern Cruise_Control_t CruiseControl;

#endif /* ESCOOTER_SAFETYSETTING_H_ */
