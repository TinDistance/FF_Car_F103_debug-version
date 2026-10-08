#include "app.h"
#include "servo.h"

void APP_Loop(void)
{
    /* 主循环：协议解析 + 状态机 + 安全停机，后续填充 */
	static uint16_t angle = 0;   /* 当前角度，static 保证每次进循环值不会丢失 */
    static uint8_t up = 1;       /* 扫描方向：1=角度增大，0=角度减小 */

    /* 把当前角度写到 1 号舵机 */
    SERVO_SetAngle(1, angle);

    HAL_Delay(10);

    /* 更新角度，实现来回扫描  */
    if (up) {
        angle += 2;              /* 每次加 2 度 */
        if (angle >= 180) {
            up = 0;              /* 到 180°，改为往回扫 */
        }
    } else {
        angle -= 2;              /* 每次减 2 度 */
        if (angle == 0) {
            up = 1;              /* 回到 0°，改为正向扫 */
        }
    }
}