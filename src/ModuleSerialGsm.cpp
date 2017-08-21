#include <ModuleSerialGsm.h>

ModuleSerialGsm::ModuleSerialGsm(ModuleSerialCore *core)
{
    this->core = core;
}

ModuleSerialGsm::BatteryStatus ModuleSerialGsm::currentBatteryStatus()
{
    char response[50] = "";
    core->writeCommand("AT+CBC", response, 50, 2000);

    ModuleSerialGsm::BatteryStatus batteryStatus;

    if (strstr(response, "+CBC:") != NULL)
    {
        parseBatteryStatus(&batteryStatus, response);
    }

    return batteryStatus;
}

int ModuleSerialGsm::currentSignalQuality()
{
    char response[30] = "";
    core->writeCommand("AT+CSQ", response, 30, 2000);

    int signalQuality = 0;

    if (strstr(response, "+CSQ:") != NULL)
    {
        parseSignalQuality(&signalQuality, response);
    }

    return signalQuality;
}

void ModuleSerialGsm::currentNetwork(char *output)
{
    char response[50] = "";
    core->writeCommand("AT+COPS?", response, 50, 2000);

    if (strstr(response, "+COPS:") != NULL)
    {
        parseNetwork(output, response);
    }
}

bool ModuleSerialGsm::enterPin(const char *pin)
{
    if (!core->writeCommand("AT+CPIN?", "READY", 2000))
    {
        char command[25] = "";
        sprintf(command, "AT+CPIN=\"%s\"", pin);

        return core->writeCommand(command, "OK", 2000);
    }

    return true;
}

void ModuleSerialGsm::registerOnNetwork()
{
    while(
        !core->writeCommand("AT+CREG?", "+CREG: 0,1", 2000) && 
        !core->writeCommand("AT+CREG?", "+CREG: 0,5", 2000));
}

void ModuleSerialGsm::parseBatteryStatus(ModuleSerialGsm::BatteryStatus *batteryStatus, char *response)
{
    char *pch;
    pch = strtok(response, " ,");

    pch = strtok(NULL, " ,");
    batteryStatus->mode = atoi(pch);

    pch = strtok(NULL, " ,");
    batteryStatus->capacity = atoi(pch);

    pch = strtok(NULL, " ,");
    batteryStatus->voltage = atoi(pch);
}

void ModuleSerialGsm::parseSignalQuality(int *signalQuality, char *response)
{
    char *pch;
    pch = strtok(response, " ,");

    pch = strtok(NULL, " ,");
    *signalQuality = atoi(pch);
}

void ModuleSerialGsm::parseNetwork(char *network, char *response)
{
    char *pch;
    pch = strtok(response, " ,");

    pch = strtok(NULL, " ,");
    pch = strtok(NULL, " ,");

    pch = strtok(NULL, "\n");
    strcpy(network, pch);
    network[strlen(network) - 1] = '\0';
}
