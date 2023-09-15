/*
 * escooter_control.h
 *
 *  Created on: 7 Jul 2023
 *      Author: Tim
 */

#ifndef ESCOOTER_ESCOOTER_CONTROL_H_
#define ESCOOTER_ESCOOTER_CONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

void ESCOOTER_init(void);

void ESCOOTER_RunCoreTask(void);

/*ESCOOTER_StopCoreTask() is triggered in the following conditions:
 * 1. UART/Heart-beat Failure
 * 2. Motor Control System Errors
 * 3. Brake and Throttle Errors
 * 4. Power Off the E-Scooter
 * */
void ESCOOTER_StopCoreTask(void);

#ifdef __cplusplus
}
#endif
#endif /* ESCOOTER_ESCOOTER_CONTROL_H_ */
