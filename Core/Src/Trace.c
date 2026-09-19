#include "main.h"
#include "Car.h"
#include "Trace.h"

extern volatile float vx, vy, omega;    /* main.c 里的底盘速度 */

/* 每路探头的位置权重：最左 -3.5，最右 +3.5，中间两路在 0 的两边。
 * 线正好压在正中间时左右权重抵消，偏差就是 0 */
static const float TraceWeight[8] = {-3.5f, -2.5f, -1.5f, -0.5f,
                                      0.5f,  1.5f,  2.5f,  3.5f};

uint8_t TraceValue[8];                  /* 8 路探头，非 0 = 压到黑线 */
float   TraceError;                     /* 偏差，-1 ~ +1 */

static float LastSign = 1.0f;           /* 上次线偏在哪边，丢线时照着找 */

/* 只管算，不管收：rx_data 里是已经收到的一帧，由串口那边填好 */
void Trace(void)
{
    uint8_t i;
    uint8_t OnCnt = 0;
    float   Sum = 0;

    /* ---------- 0. 帧头帧尾不对，说明这帧没收全，或者收的不是这个模块的数据 ---------- */
    if (rx_data[0] != '$' || rx_data[TRACE_FRAME_LEN - 1] != '#')
    {
        return;                         /* 这次不动，保留上一帧的动作 */
    }

    /* ---------- 1. 把 8 路状态抠出来 ----------
     * 每路的值固定在这几个位置上：
     *   $ D , x 1 : 0 , x 2 : 1 , x 3 : 1 ...
     *   0 1 2 3 4 5 6 7 8 9 10 11 12 ...
     * 也就是 6、11、16、21、26、31、36、41，每隔 5 个字节一个。
     * 模块是 0 = 压到线，这里翻成 1 = 压到线，后面看着顺，
     * 翻完下面就不用再管极性了 */
    for (i = 0; i < 8; i++)
    {
        TraceValue[i] = ((rx_data[6 + i * 5] - '0') == TRACE_ON_LEVEL) ? 1 : 0;
    }

    /* ---------- 2. 压到黑线的那几路说了算，按位置加权求线的中心 ---------- */
    for (i = 0; i < 8; i++)
    {
        if (TraceValue[i])
        {
            OnCnt++;
            Sum += TraceWeight[i];
        }
    }

    /* ---------- 3. 一路都没压到：线丢了 ---------- */
    if (OnCnt == 0)
    {
        vx = 0;
        vy = 0;
        omega = TRACE_LOST_OMEGA * LastSign;    /* 照着上次偏的方向原地找 */
        Car_Speed(vx, vy, omega);
        return;
    }

    /* ---------- 4. 8 路全压到黑线：横着的黑线（十字路口或者终点线）---------- */
    if (OnCnt == 8)
    {
        vx = 0;
        vy = 0;
        omega = TRACE_LOST_OMEGA;
        Car_Speed(vx, vy, omega);
        return;
    }

    /* ---------- 5. 正常循迹：偏差做 P 控制 ---------- */
    TraceError = Sum / (float)OnCnt;        /* -3.5 ~ +3.5，负数 = 线在左边 */
    TraceError = TraceError / 3.5f;         /* 归一化到 -1 ~ +1 */

    vx = TRACE_SPEED;
    vy = 0;
    omega = TRACE_KP * TraceError * TRACE_SIGN;   /* 线在左就往左转 */

    if (omega >  TRACE_OMEGA_MAX) omega =  TRACE_OMEGA_MAX;
    if (omega < -TRACE_OMEGA_MAX) omega = -TRACE_OMEGA_MAX;

    LastSign = (TraceError >= 0) ? 1.0f : -1.0f;

    Car_Speed(vx, vy, omega);               /* 这一步原来漏了，车才不动 */
}
