#include "bsp.h"
#include "servo.h"

void BSP_Init(void)
{
     SERVO_Init();          /* 舵机：启动 PWM 并回中位 */
}