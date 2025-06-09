#ifndef EMBUX_GLOBAL_DEFINE
#define EMBUX_GLOBAL_DEFINE

#define EMBUX_EXIT_FAILURE  -1
#define EMBUX_EXIT_SUCCESS  0

#ifdef __cplusplus
namespace neoEmbUx {
    const int IN        = GPIOD_LINE_DIRECTION_INPUT;
    const int OUT       = GPIOD_LINE_DIRECTION_OUTPUT;
    const int LOW       = 0;
    const int HIGH      = 1;
    const int RISING    = 1;
    const int FALLING   = 2;
}
extern "C" { 
#endif
const int NEOEMBUX_IN       = GPIOD_LINE_DIRECTION_INPUT;
const int NEOEMBUX_OUT      = GPIOD_LINE_DIRECTION_OUTPUT;
const int NEOEMBUX_LOW      = 0;
const int NEOEMBUX_HIGH     = 1;
const int NEOEMBUX_RISING   = 1;
const int NEOEMBUX_FALLING  = 2;
#ifdef __cplusplus
}                           // 结束 extern "C" 块
#endif

#endif