#include <ModuleSerialGsm_Phone.h>

ModuleSerialGsm_Phone::ModuleSerialGsm_Phone(ModuleSerialCore *core) : ModuleSerialGsm(core)
{
}

int ModuleSerialGsm_Phone::enable(const char *pin)
{
    if (!ModuleSerialGsm::enterPin(pin)) 
    {
        return GSM_FAIL;
    }

    delay(2000);
    ModuleSerialGsm::registerOnNetwork();

    return GSM_ENABLED;
}

bool ModuleSerialGsm_Phone::callAvailable()
{
    char command[100] = "";
    int i = 0;

    while (ModuleSerialGsm::core->SoftwareSerial::available())
    {
        command[i++] = ModuleSerialGsm::core->SoftwareSerial::read();
        if (i >= 99) 
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
    char command[40] = "";
    sprintf(command, "ATD%s;", number);

    if (!core->writeCommand(command, "+COLP:", timeout))
    {
        callDrop();
    }

    delay(250);
}

void ModuleSerialGsm_Phone::callAnswer()
{
    core->writeCommand("ATA", "OK", 2000);  
}

void ModuleSerialGsm_Phone::callDrop()
{
    core->writeCommand("ATH", "OK", 2000);
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

    while (call[++i] != '+' && call[i] != '\0');
    while (call[i] != '"' && call[i] != '\0')
    {
        callNumber[j++] = call[i++];
    }
}