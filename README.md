# Cross platform INDI driver for gamepads

```sh
sudo apt install libsdl2-dev

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ../
make
sudo make install

indiserver indi_gamepad
```
