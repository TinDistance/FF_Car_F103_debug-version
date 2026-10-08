#include "servo.h"
#include "tim.h"   

/*
 * 舵机编号与定时器通道的对应关系：
 *   1 -> TIM2_CH1 (PA0)
 *   2 -> TIM2_CH2 (PA1)
 *   3 -> TIM2_CH3 (PA2)
 *   4 -> TIM2_CH4 (PA3)
 *   6 -> TIM3_CH4 (PB1)
 *   5 -> PA4（该引脚没有定时器通道，暂不实现，待定）
 */

void SERVO_Init(void)
{
    /* 启动各通道的 PWM 输出。
     */
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

    /* 上电时把所有舵机摆到 90° 中位。
     */
    SERVO_SetAngle(1, 90);
    SERVO_SetAngle(2, 90);
    SERVO_SetAngle(3, 90);
    SERVO_SetAngle(4, 90);
    SERVO_SetAngle(6, 90);
}

void SERVO_SetAngle(uint8_t id, uint16_t angle_deg)
{
    uint32_t pulse;

    /* 防止角度越界：超过 180 度会算出超过 2500us 的脉宽，先把角度截断 */
    if (angle_deg > SERVO_ANGLE_MAX) {
        angle_deg = SERVO_ANGLE_MAX;
    }

    /* 角度 -> 脉宽的线性换算：
     *   pulse = 500 + angle * (2500 - 500) / 180
     */
    pulse = SERVO_PULSE_MIN +
        ((uint32_t)angle_deg * (SERVO_PULSE_MAX - SERVO_PULSE_MIN)) / SERVO_ANGLE_MAX;

    /* 调用底层函数，把换算好的脉宽写进定时器 */
    SERVO_SetPulseUs(id, (uint16_t)pulse);
}

void SERVO_SetPulseUs(uint8_t id, uint16_t pulse_us)
{
    /* 限幅：确保脉宽始终落在 500~2500us 的合法范围内 */
    if (pulse_us < SERVO_PULSE_MIN) {
        pulse_us = SERVO_PULSE_MIN;
    }
    if (pulse_us > SERVO_PULSE_MAX) {
        pulse_us = SERVO_PULSE_MAX;
    }

    
    switch (id) {
        case 1: __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse_us); break;
        case 2: __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse_us); break;
        case 3: __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pulse_us); break;
        case 4: __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, pulse_us); break;
        case 6: __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, pulse_us); break;
        default:
            /* 5 号舵机在 PA4，没有定时器通道，暂不处理 */
            break;
    }
}