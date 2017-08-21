#ifndef _ModuleSerialGsm_H
#define _ModuleSerialGsm_H

#include <ModuleSerialCore.h>

#define GSM_FAIL -1 
#define GSM_ENABLED 0

class ModuleSerialGsm
{
public:
    struct BatteryStatus
    {
        int mode;       // 0 => not charging; 1 => charging; 2 => finished charging;
        int capacity;
        int voltage;
    };

    ModuleSerialGsm(ModuleSerialCore *core);

    BatteryStatus currentBatteryStatus();
    int currentSignalQuality();
    void currentNetwork(char *output);

protected:
    ModuleSerialCore *core = nullptr;

    bool enterPin(const char *pin);
    void registerOnNetwork();

    void parseBatteryStatus(BatteryStatus *batteryStatus, char *response);
    void parseSignalQuality(int *signalQuality, char *response);
    void parseNetwork(char *network, char *response);
};

#endif 
