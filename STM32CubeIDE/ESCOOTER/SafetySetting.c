/*
 * SafetySetting.c
 *
 *  Created on: 6 Oct 2023
 *      Author: TerenceLeung
 */

#include "SafetySetting.h"

Cruise_Control_t CruiseControl = {
		.DefKpVar = (int16_t) PROPORTIONAL_GAIN,
		.DefKiVar = (int16_t) INTEGRAL_GAIN,
		.DefKdVar = (int16_t) DIFFERENTIAL_GAIN,
};
