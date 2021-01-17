# Cross platform INDI driver for gamepads

Requires a nightly build of libindi-1.8.9 or higher (or indi built from source).

## Linux (Debian or Ubuntu)

```sh
# linux
sudo apt install libsdl2-dev

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=RelWithDebInfo ../
make
sudo make install
```

## OSX

```sh
brew install sdl2

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=RelWithDebInfo ../
make
sudo make install
```
