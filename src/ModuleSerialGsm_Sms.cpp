#include <ModuleSerialGsm_Sms.h>

ModuleSerialGsm_Sms::ModuleSerialGsm_Sms(ModuleSerialCore *core) : ModuleSerialGsm(core)
{
}

int ModuleSerialGsm_Sms::enable(const char *pin)
{
    if (!ModuleSerialGsm::enterPin(pin)) 
    {
        return GSM_FAIL;
    }

    delay(2000);
    ModuleSerialGsm::registerOnNetwork();

    if (!core->writeCommand("AT+CMGF=1", "OK", 2000))
        return GSM_FAIL;
    if (!core->writeCommand("AT+CNMI=2,1,0,0,0", "OK", 2000))
        return GSM_FAIL;

    core->writeCommand("AT+CMGD=1,4", "OK", 2000);

    return GSM_ENABLED;
}

bool ModuleSerialGsm_Sms::messageAvailable()
{
    char response[200] = "";
    core->writeCommand("AT+CMGR=1", response, 200, 2000);

    if (strstr(response, "+CMGR:") != NULL)
    {
        parseMessage(response);
        return true;        
    }
    return false;
}

void ModuleSerialGsm_Sms::messageSend(const char *number, const char *content)
{
    char command[40] = "";
    sprintf(command, "AT+CMGS=\"%s\"", number);

    core->writeCommand(command, ">", 2000);
    
    core->print(content);
    core->write(0x1A);

    delay(250);
}

void ModuleSerialGsm_Sms::messageFlush()
{
    core->writeCommand("AT+CMGD=1,3", "OK", 2000);  // Delete 'read', 'sent' and 'saved but unsent' messages.
}

void ModuleSerialGsm_Sms::receivedNumber(char *output, int size)
{
    int i = 0;
    while (messageNumber[i] != '\0' && i < size)
    {
        output[i] = messageNumber[i];
        ++i;
    }
    output[i] = '\0';
}

void ModuleSerialGsm_Sms::receivedContent(char *output, int size)
{
    int i = 0;
    while (messageContent[i] != '\0' && i < size)
    {
        output[i] = messageContent[i];
        ++i;
    }
    output[i] = '\0';
}

void ModuleSerialGsm_Sms::parseMessage(char *message)
{
    memset(messageNumber, '\0', MESSAGE_NUMBER_LENGTH);
    memset(messageContent, '\0', MESSAGE_CONTENT_LENGTH);

    int i = 32;
    int j = 0;
    int k = 0;

    while (message[++i] != '+' && message[i] != '\0');
    while (message[i] != '"' && message[i] != '\0')
    {
        messageNumber[j++] = message[i++];
    }
    while (message[i++] != '\n' && message[i] != '\0');
    while (message[i] != '\n' && message[i] != '\0')
    {
        messageContent[k++] = message[i++];
    }
}
