/*
 * ERROR_HANDLER.h
 *
 *  Created on: 20 Dec 2023
 *      Author: TerenceLeung
 */

#ifndef ESCOOTER_ERROR_HANDLER_H_
#define ESCOOTER_ERROR_HANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define ERROR_CODE_PACKET             0x4A
#define ERROR_MSG_HANDLE_LENGTH       0x01
#define SYSTEM_NORMAL                 0x00   /*BOOT Completed*/
#define CONNECTION_FAIL               0x0A
#define SYSTEM_ABNORMAL               0x0B

#define ABNORMAL_BATTERY_TEMPERATURE  0x1A
#define BMS_COMMUNITATION_ERROR       0x1C
#define ABNORMAL_CURRENT              0x2A
#define GATE_DRIVER_ABNORMAL          0x2C
#define MOSFET_ABNORMAL               0x2E
#define HALL_SENSOR_FAIL              0x3A
#define ABNORMAL_TEMPERATURE          0x3C

extern void SET_ERROR_CODE(uint16_t error_code);

extern uint8_t GET_ERROR_CODE();

#ifdef __cplusplus
}
#endif
#endif /* ESCOOTER_ERROR_HANDLER_H_ */
