# NeoEmbUX
这是一个基于gpiod的GPIO驱动库。

安装：
```  shell
apt update && apt upgrade
apt install -y gpiod libgpiod-dev git cmake
git clone https://github.com/SivonZn/neoembux.git
cd neoembux
mkdir build && cd build
cmake .. && make 
sudo make install
sudo ldconfig
```

C++ 用法用法如下：
``` C++
int neoEmbUx::setup()
int neoEmbUx::readPin(int pin)
int neoEmbUx::setPin(int pin, int mode, ...) // 若为输出，则需指定HIGH或LOW
bool neoEmbUx::checkPin(int pin) 
void neoEmbUx::release()
```
``` shell
gcc <your_program_code> -o <your_program_name> -lneoembux
sudo ./<your_program_name>
```
C 用法用法如下：
``` C
// C 用法
int neoEmbUx_ioSetup(void);
int neoEmbUx_readPin(int pin);
int neoEmbUx_setPin(int pin, int mode, ...);
void neoEmbUx_ioRelease(void);
```
``` shell
gcc <your_program_code> -o <your_program_name> -lneoembux
sudo ./<your_program_name>
```