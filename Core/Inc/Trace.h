#ifndef __TRACE_H
#define __TRACE_H

#include <stdint.h>

/* ---------- 一帧长这样，一共 43 个字节 ----------
 *
 *   $D,x1:0,x2:1,x3:1,x4:1,x5:1,x6:1,x7:1,x8:1#
 *   $D,x1:1,x2:1,x3:1,x4:0,x5:0,x6:1,x7:1,x8:1#
 *
 *   收到帧头 $ 之后数 42 个字节，最后一个是 #
 *   模块压到黑线那一位是 0，白底是 1
 *   要是你的模块反过来（压到黑线是 1），把下面 TRACE_ON_LEVEL 改成 1
 */
#define TRACE_FRAME_LEN  43
#define TRACE_ON_LEVEL   0

/* ---------- 要调的参数，就这三个 ---------- */

#define TRACE_SPEED      0.5f    /* 直线速度，m/s */
#define TRACE_KP         1.57f   /* 偏差 -> 角速度，越大转得越狠 */
#define TRACE_SIGN       1.0f    /* 车往反方向转就改成 -1.0f */

/* ---------- 一般不用改的 ---------- */

#define TRACE_OMEGA_MAX  1.57f   /* 角速度上限，rad/s，等于最大转向能力 */
#define TRACE_LOST_OMEGA 1.0f    /* 丢线时找线的角速度 */

/* 已经收到的一帧原始数据，由串口那边填，循迹这边只读 */
extern char rx_data[TRACE_FRAME_LEN];

/* 8 路探头的值，下标 0 = 最左 ... 7 = 最右。非 0 = 压到黑线 */
extern uint8_t TraceValue[8];

/* 最近这一帧算出的偏差，-1 ~ +1，负数 = 线在左边。调试看这个 */
extern float TraceError;

/* 循迹：解析 rx_data 里的一帧 -> 算偏差 -> 驱动底盘。
 * 只管算不管收，放在主循环里一直调用就行 */
void Trace(void);

#endif
