#ifndef __SERVO_H
#define __SERVO_H

#include "main.h"

#define SERVO_PULSE_MIN   500u
#define SERVO_PULSE_MAX   2500u
#define SERVO_ANGLE_MAX   180u

void SERVO_Init(void);
void SERVO_SetAngle(uint8_t id, uint16_t angle_deg);
void SERVO_SetPulseUs(uint8_t id, uint16_t pulse_us);

#endif