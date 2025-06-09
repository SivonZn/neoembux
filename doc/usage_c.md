## C 用法用法如下：
``` C
// C 用法
int neoEmbUx_ioSetup(void);
int neoEmbUx_readPin(int pin);
int neoEmbUx_setPin(int pin, int mode, ...);
bool neoEmbUx_checkPin(int pin) 
void neoEmbUx_ioRelease(void);
```
## 编译命令如下：
``` shell
gcc <your_program_code> -o <your_program_name> -lneoembux
sudo ./<your_program_name>
```