#ifndef EMBUX_GLOBAL_DEFINE
#define EMBUX_GLOBAL_DEFINE

#define EMBUX_EXIT_FAILURE  -1
#define EMBUX_EXIT_SUCCESS  0

#ifdef __cplusplus
namespace neoEmbUx {
    const int IN    = 0;
    const int OUT   = 1;
    const int LOW   = 0;
    const int HIGH  = 1;
}
extern "C" { 
#endif
const int NEOEMBUX_IN   = 0;
const int NEOEMBUX_OUT  = 1;
const int NEOEMBUX_LOW  = 0;
const int NEOEMBUX_HIGH = 1;
#ifdef __cplusplus
}                           // 结束 extern "C" 块
#endif

#endif