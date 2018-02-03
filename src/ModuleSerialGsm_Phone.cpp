#include "ModuleSerialGsm_Phone.h"

ModuleSerialGsm_Phone::ModuleSerialGsm_Phone(ModuleSerialCore *core) : ModuleSerialGsm(core)
{
}

int ModuleSerialGsm_Phone::enable(const char *pin)
{
    if (!ModuleSerialGsm::enterPin(pin)) 
    {
        return GSM_FAIL;
    }

    delay(DELAY_LONG);
    ModuleSerialGsm::registerOnNetwork();

    return GSM_ENABLED;
}

bool ModuleSerialGsm_Phone::callAvailable()
{
    char command[BUF_MEDIUM_LEN] = "";
    int i = 0;

    while (ModuleSerialGsm::core->available())
    {
        command[i++] = ModuleSerialGsm::core->read();

        if (i >= BUF_MEDIUM_LEN - 1) 
        {
            command[i] = '\0';

            return false;
        }
    }

    if (strstr(command, "RING"))
    {
        parseCall(command); 
        return true;
    }

    return false;
}

void ModuleSerialGsm_Phone::callMake(const char *number, unsigned long timeout)
{
    char command[BUF_SHORT_LEN] = "";
    sprintf(command, "ATD%s;", number);

    if (!core->writeCommand(command, "+COLP:", timeout))
    {
        callDrop();
    }

    delay(DELAY_SHORT);
}

void ModuleSerialGsm_Phone::callAnswer()
{
    core->writeCommand("ATA", "OK", TIMEOUT);  
}

void ModuleSerialGsm_Phone::callDrop()
{
    core->writeCommand("ATH", "OK", TIMEOUT);
}

void ModuleSerialGsm_Phone::receivedNumber(char *output, int size)
{
    int i = 0;
    while (callNumber[i] != '\0' && i < size)
    {
        output[i] = callNumber[i];
        ++i;
    }
    output[i] = '\0';
}

void ModuleSerialGsm_Phone::parseCall(char *call)
{
    memset(callNumber, '\0', CALL_NUMBER_LENGTH);

    int i = 12;
    int j = 0;
    int k = 0;

    while (call[++i] != '+' && call[i] != '\0')
    ;
    
    while (call[i] != '"' && call[i] != '\0')
    {
        callNumber[j++] = call[i++];
    }
}