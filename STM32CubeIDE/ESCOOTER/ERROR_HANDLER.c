/*
 * ERROR_HANDLER.c
 *
 *  Created on: 20 Dec 2023
 *      Author: TerenceLeung
 */
#include "ERROR_HANDLER.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>


uint8_t MSG_CHECK(uint8_t *msg, uint8_t size)
{
	uint16_t total = 0;
	uint8_t n = 0;
	while(n != size)
	{
		total += msg[n];
		n++;
	}
	return (total & 0xFF) + ((total >> 8) & 0xFF);
}

void ERROR_HANDLE_MSG(uint8_t error_code)
{
    uint8_t *txFrame = (uint8_t*)malloc(sizeof(uint8_t*)*(ERROR_MSG_HANDLE_LENGTH+3));
    txFrame[0] = ERROR_CODE_PACKET;
    txFrame[1] = ERROR_MSG_HANDLE_LENGTH;
    txFrame[2] = error_code;
    txFrame[3] = MSG_CHECK(txFrame, 3);
    send_error_message(txFrame,ERROR_MSG_HANDLE_LENGTH+3);
}
