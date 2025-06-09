## C++ 用法用法如下：
``` C++
int neoEmbUx::ioSetup()
int neoEmbUx::readPin(int pin)
int neoEmbUx::setPin(int pin, int mode, ...) // 若为输出，则需指定HIGH或LOW
bool neoEmbUx::checkPin(int pin) 
void neoEmbUx::ioRelease()
```
## 编译命令如下：
``` shell
gcc <your_program_code> -o <your_program_name> -lneoembux
sudo ./<your_program_name>
```