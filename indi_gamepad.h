#pragma once

#include <libindi/defaultdevice.h>
#include <SDL2/SDL_gamecontroller.h>

class GamePad : public INDI::DefaultDevice
{
public:
    GamePad();
    virtual ~GamePad() = default;

    virtual const char *getDefaultName() override;

    virtual bool initProperties() override;
    virtual bool updateProperties() override;

    virtual void ISGetProperties(const char *dev);
    virtual bool ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[],
                             int n) override;

    virtual void TimerHit() override;

    virtual bool Connect() override;
    virtual bool Disconnect() override;

protected:
    virtual bool saveConfigItems(FILE *fp) override;

private:
    void refreshGamepadList();

    SDL_GameController *gamepad = nullptr;

    std::vector<int> joystickIndexes;

    ISwitch RefreshS[1];
    ISwitchVectorProperty RefreshSP;

    ISwitch *GamePadS = nullptr;
    ISwitchVectorProperty GamePadsSP;

    enum
    {
        JOYSTICK_MAGNITUDE,
        JOYSTICK_ANGLE,
        JOYSTICK_N,
    };

    INumber JoystickLeftN[JOYSTICK_N];
    INumberVectorProperty JoystickLeftNP;

    INumber JoystickRightN[JOYSTICK_N];
    INumberVectorProperty JoystickRightNP;

    INumber AxisN[SDL_CONTROLLER_AXIS_MAX];
    INumberVectorProperty AxisNP;

    ISwitch ButtonS[SDL_CONTROLLER_BUTTON_MAX];
    ISwitchVectorProperty ButtonSP;
};
