/*
 * ERROR_HANDLER.c
 *
 *  Created on: 20 Dec 2023
 *      Author: TerenceLeung
 */
#include "ERROR_HANDLER.h"
#include "mc_type.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

/*System is normal in default*/
uint8_t ERROR_OCCURRED = MC_NO_ERROR;
uint8_t ERROR_CODE = SYSTEM_NORMAL;

void SET_ERROR_CODE(uint16_t error_code)
{
	ERROR_OCCURRED = error_code;
}

uint8_t GET_ERROR_CODE()
{
	if(ERROR_OCCURRED == MC_NO_ERROR)
	{
		ERROR_CODE = SYSTEM_NORMAL;
	}
	else if(ERROR_OCCURRED == MC_SPEED_FDBK)
	{
		ERROR_CODE = HALL_SENSOR_FAIL;
	}
	else if(ERROR_OCCURRED == MC_OVER_VOLT || ERROR_OCCURRED == MC_UNDER_VOLT)
	{
		ERROR_CODE = ABNORMAL_CURRENT;
	}
	else if(ERROR_OCCURRED == MC_OVER_TEMP)
	{
		ERROR_CODE = ABNORMAL_BATTERY_TEMPERATURE;
	}
    return ERROR_CODE;
}
