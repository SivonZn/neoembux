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