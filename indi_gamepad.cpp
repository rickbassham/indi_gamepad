#include <cstring>
#include <termios.h>

#include <libindi/indicom.h>
#include <SDL2/SDL.h>

#include "config.h"
#include "indi_gamepad.h"

static std::unique_ptr<GamePad> mydriver(new GamePad());

void ISGetProperties(const char *dev)
{
    mydriver->ISGetProperties(dev);
}

void ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n)
{
    mydriver->ISNewSwitch(dev, name, states, names, n);
}

void ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n)
{
    mydriver->ISNewText(dev, name, texts, names, n);
}

void ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n)
{
    mydriver->ISNewNumber(dev, name, values, names, n);
}

void ISNewBLOB(const char *dev, const char *name, int sizes[], int blobsizes[], char *blobs[],
               char *formats[], char *names[], int n)
{
    mydriver->ISNewBLOB(dev, name, sizes, blobsizes, blobs, formats, names, n);
}

void ISSnoopDevice(XMLEle *root)
{
    mydriver->ISSnoopDevice(root);
}

double MAX_MAG = sqrt(pow(32768, 2) * 2);

double scaleMagnitude(double mag)
{
    return mag / MAX_MAG;
}

GamePad::GamePad()
{
    setVersion(CDRIVER_VERSION_MAJOR, CDRIVER_VERSION_MINOR);
}

const char *GamePad::getDefaultName()
{
    return "Joystick";
}

bool GamePad::initProperties()
{
    // initialize the parent's properties first
    INDI::DefaultDevice::initProperties();

    SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);

    // We will poll for udpates instead of relying on the events.
    SDL_JoystickEventState(SDL_DISABLE);
    SDL_GameControllerEventState(SDL_DISABLE);

    IUFillSwitch(&RefreshS[0], "REFRESH", "Refresh Gamepad List", ISS_OFF);
    IUFillSwitchVector(&RefreshSP, RefreshS, 1, getDeviceName(), "REFRESH", "Refresh", MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_IDLE);
    defineSwitch(&RefreshSP);

    IUFillNumber(&JoystickLeftN[JOYSTICK_MAGNITUDE], "JOYSTICK_MAGNITUDE", "Magnitude", "%g", 0, 1, 0, 0);
    IUFillNumber(&JoystickLeftN[JOYSTICK_ANGLE], "JOYSTICK_ANGLE", "Angle", "%g", 0, 360, 0, 0);
    IUFillNumberVector(&JoystickLeftNP, JoystickLeftN, JOYSTICK_N, getDeviceName(), "JOYSTICK_1", "Left Joystick", MAIN_CONTROL_TAB, IP_RO, 0, IPS_IDLE);

    IUFillNumber(&JoystickRightN[JOYSTICK_MAGNITUDE], "JOYSTICK_MAGNITUDE", "Magnitude", "%g", -32768, 32767, 0, 0);
    IUFillNumber(&JoystickRightN[JOYSTICK_ANGLE], "JOYSTICK_ANGLE", "Angle", "%g", 0, 360, 0, 0);
    IUFillNumberVector(&JoystickRightNP, JoystickRightN, JOYSTICK_N, getDeviceName(), "JOYSTICK_2", "Right Joystick", MAIN_CONTROL_TAB, IP_RO, 0, IPS_IDLE);

    IUFillNumber(&AxisN[SDL_CONTROLLER_AXIS_LEFTX], "AXIS_1", "Left X", "%.0lf", -32768, 32767, 0, 0);
    IUFillNumber(&AxisN[SDL_CONTROLLER_AXIS_LEFTY], "AXIS_2", "Left Y", "%.0lf", -32768, 32767, 0, 0);
    IUFillNumber(&AxisN[SDL_CONTROLLER_AXIS_RIGHTX], "AXIS_3", "Right X", "%.0lf", -32768, 32767, 0, 0);
    IUFillNumber(&AxisN[SDL_CONTROLLER_AXIS_RIGHTY], "AXIS_4", "Right Y", "%.0lf", -32768, 32767, 0, 0);
    IUFillNumber(&AxisN[SDL_CONTROLLER_AXIS_TRIGGERLEFT], "AXIS_5", "Left Trigger", "%.0lf", -32768, 32767, 0, 0);
    IUFillNumber(&AxisN[SDL_CONTROLLER_AXIS_TRIGGERRIGHT], "AXIS_6", "Right Trigger", "%.0lf", -32768, 32767, 0, 0);

    IUFillNumberVector(&AxisNP, AxisN, SDL_CONTROLLER_AXIS_MAX, getDeviceName(), "JOYSTICK_AXES", "Axes", MAIN_CONTROL_TAB, IP_RO, 0, IPS_IDLE);

    IUFillSwitch(&ButtonS[SDL_CONTROLLER_BUTTON_A], "BUTTON_1", "A", ISS_OFF);
    IUFillSwitch(&ButtonS[SDL_CONTROLLER_BUTTON_B], "BUTTON_2", "B", ISS_OFF);
    IUFillSwitch(&ButtonS[SDL_CONTROLLER_BUTTON_X], "BUTTON_3", "X", ISS_OFF);
    IUFillSwitch(&ButtonS[SDL_CONTROLLER_BUTTON_Y], "BUTTON_4", "Y", ISS_OFF);
    IUFillSwitch(&ButtonS[SDL_CONTROLLER_BUTTON_BACK], "BUTTON_5", "Back", ISS_OFF);
    IUFillSwitch(&ButtonS[SDL_CONTROLLER_BUTTON_GUIDE], "BUTTON_6", "Guide", ISS_OFF);
    IUFillSwitch(&ButtonS[SDL_CONTROLLER_BUTTON_START], "BUTTON_7", "Start", ISS_OFF);
    IUFillSwitch(&ButtonS[SDL_CONTROLLER_BUTTON_LEFTSTICK], "BUTTON_8", "Left Stick", ISS_OFF);
    IUFillSwitch(&ButtonS[SDL_CONTROLLER_BUTTON_RIGHTSTICK], "BUTTON_9", "Right Stick", ISS_OFF);
    IUFillSwitch(&ButtonS[SDL_CONTROLLER_BUTTON_LEFTSHOULDER], "BUTTON_10", "Left Shoulder", ISS_OFF);
    IUFillSwitch(&ButtonS[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER], "BUTTON_11", "Right Shoulder", ISS_OFF);
    IUFillSwitch(&ButtonS[SDL_CONTROLLER_BUTTON_DPAD_UP], "BUTTON_12", "DPad Up", ISS_OFF);
    IUFillSwitch(&ButtonS[SDL_CONTROLLER_BUTTON_DPAD_DOWN], "BUTTON_13", "DPad Down", ISS_OFF);
    IUFillSwitch(&ButtonS[SDL_CONTROLLER_BUTTON_DPAD_LEFT], "BUTTON_14", "DPad Left", ISS_OFF);
    IUFillSwitch(&ButtonS[SDL_CONTROLLER_BUTTON_DPAD_RIGHT], "BUTTON_15", "DPad Right", ISS_OFF);

    IUFillSwitchVector(&ButtonSP, ButtonS, SDL_CONTROLLER_BUTTON_MAX, getDeviceName(), "JOYSTICK_BUTTONS", "Buttons", MAIN_CONTROL_TAB, IP_RO, ISR_NOFMANY, 0, IPS_IDLE);

    refreshGamepadList();

    addAuxControls();

    setDefaultPollingPeriod(100);

    return true;
}

void GamePad::ISGetProperties(const char *dev)
{
    DefaultDevice::ISGetProperties(dev);
}

bool GamePad::updateProperties()
{
    INDI::DefaultDevice::updateProperties();

    if (isConnected())
    {
        defineNumber(&JoystickLeftNP);
        defineNumber(&JoystickRightNP);
        defineNumber(&AxisNP);
        defineSwitch(&ButtonSP);

        RefreshSP.p = IP_RO;
    }
    else
    {
        deleteProperty(JoystickLeftNP.name);
        deleteProperty(JoystickRightNP.name);
        deleteProperty(AxisNP.name);
        deleteProperty(ButtonSP.name);

        RefreshSP.p = IP_RW;
    }

    IDSetSwitch(&RefreshSP, nullptr);

    return true;
}

bool GamePad::ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[],
                          int n)
{
    if (dev != nullptr && strcmp(dev, getDeviceName()) == 0)
    {
        if (strcmp(name, "GAMEPADS") == 0)
        {
            IUUpdateSwitch(&GamePadsSP, states, names, n);

            GamePadsSP.s = IPS_OK;

            IDSetSwitch(&GamePadsSP, nullptr);

            setConnected(Connect());
            updateProperties();

            return true;
        }

        if (strcmp(name, "REFRESH") == 0)
        {
            RefreshSP.s = IPS_IDLE;

            refreshGamepadList();

            RefreshS->s = ISS_OFF;

            IDSetSwitch(&RefreshSP, nullptr);

            return true;
        }
    }

    return INDI::DefaultDevice::ISNewSwitch(dev, name, states, names, n);
}

bool GamePad::saveConfigItems(FILE *fp)
{
    INDI::DefaultDevice::saveConfigItems(fp);

    return true;
}

bool GamePad::Connect()
{
    int index = IUFindOnSwitchIndex(&GamePadsSP);

    if (index < 0 && GamePadsSP.nsp == 1)
    {
        GamePadS[0].s = ISS_ON;
        IDSetSwitch(&GamePadsSP, nullptr);
        index = 0;
    }

    if (index >= 0)
    {
        gamepad = SDL_GameControllerOpen(joystickIndexes[index]);

        if (gamepad)
        {
            SetTimer(POLLMS);
            return true;
        }
    }

    return false;
}

bool GamePad::Disconnect()
{
    if (gamepad)
    {
        SDL_GameControllerClose(gamepad);
        gamepad = nullptr;
    }

    setConnected(false);
    updateProperties();

    return true;
}

void GamePad::TimerHit()
{
    if (!isConnected())
        return;

    if (SDL_GameControllerGetAttached(gamepad) == SDL_FALSE)
    {
        setConnected(false);
        IUResetSwitch(&GamePadsSP);
        updateProperties();
        return;
    }

    SDL_JoystickUpdate();
    SDL_GameControllerUpdate();

    bool axisChanged = false;
    bool buttonChanged = false;

    for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; i++)
    {
        int val = SDL_GameControllerGetAxis(gamepad, (SDL_GameControllerAxis)i);

        if (abs(val) < 100)
        {
            val = 0;
        }

        if (AxisN[i].value != val)
        {
            axisChanged = true;
        }

        AxisN[i].value = val;
    }

    JoystickLeftN[JOYSTICK_ANGLE].value = range360(atan2(AxisN[SDL_CONTROLLER_AXIS_LEFTY].value, AxisN[SDL_CONTROLLER_AXIS_LEFTX].value) * (180.0 / M_PI));
    JoystickRightN[JOYSTICK_ANGLE].value = range360(atan2(AxisN[SDL_CONTROLLER_AXIS_RIGHTY].value, AxisN[SDL_CONTROLLER_AXIS_RIGHTX].value) * (180.0 / M_PI));

    JoystickLeftN[JOYSTICK_MAGNITUDE].value = std::min(sqrt(pow(AxisN[SDL_CONTROLLER_AXIS_LEFTY].value, 2) + pow(AxisN[SDL_CONTROLLER_AXIS_LEFTX].value, 2)) / 32767.0, 1.0);
    JoystickRightN[JOYSTICK_MAGNITUDE].value = std::min(sqrt(pow(AxisN[SDL_CONTROLLER_AXIS_RIGHTY].value, 2) + pow(AxisN[SDL_CONTROLLER_AXIS_RIGHTX].value, 2)) / 32767.0, 1.0);

    if (axisChanged)
    {
        IDSetNumber(&AxisNP, nullptr);
        IDSetNumber(&JoystickLeftNP, nullptr);
        IDSetNumber(&JoystickRightNP, nullptr);
    }

    for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
    {
        ISState val = SDL_GameControllerGetButton(gamepad, (SDL_GameControllerButton)i) == 1 ? ISS_ON : ISS_OFF;

        if (ButtonS[i].s != val)
        {
            buttonChanged = true;
        }

        ButtonS[i].s = val;
    }

    if (buttonChanged)
    {
        IDSetSwitch(&ButtonSP, nullptr);
    }

    SetTimer(POLLMS);
}

void GamePad::refreshGamepadList()
{
    deleteProperty(GamePadsSP.name);

    SDL_JoystickUpdate();
    SDL_GameControllerUpdate();

    joystickIndexes.clear();

    IDLog("joysticks count: %d\n", SDL_NumJoysticks());

    for (int i = 0; i < SDL_NumJoysticks(); ++i)
    {
        if (SDL_IsGameController(i))
        {
            joystickIndexes.push_back(i);

            IDLog("found gamecontroller at index %d: %s\n", i, SDL_GameControllerNameForIndex(i));
        }
    }

    char propName[16] = {0};
    GamePadS = new ISwitch[joystickIndexes.size()];

    for (size_t i = 0; i < joystickIndexes.size(); i++)
    {
        snprintf(propName, 16, "GAMEPAD_%ld", i);

        IUFillSwitch(
            &GamePadS[i],
            propName,
            SDL_GameControllerNameForIndex(joystickIndexes[i]),
            ISS_OFF);
    }

    IUFillSwitchVector(
        &GamePadsSP,
        GamePadS,
        joystickIndexes.size(),
        getDeviceName(),
        "GAMEPADS",
        "GamePads",
        MAIN_CONTROL_TAB,
        IP_RW,
        ISR_1OFMANY,
        60,
        IPS_IDLE);

    defineSwitch(&GamePadsSP);
}