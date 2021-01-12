# Cross platform INDI driver for gamepads

## Linux (Debian or Ubuntu)

```sh
# linux
sudo apt install libsdl2-dev

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ../
make
sudo make install

indiserver indi_gamepad
```

## OSX

```sh
brew install sdl2

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Debug ../
make
sudo make install

cp /usr/local/bin/indi_gamepad /Applications/KStars.app/Contents/MacOS/indi
cp /usr/local/share/indi/indi_gamepad.xml /Applications/KStars.app/Contents/Resources/DriverSupport

indiserver -p 7625 indi_gamepad
```

You'll need to connect remote to it, since KStars seems to be filtering out any
Joysticks on Mac.
