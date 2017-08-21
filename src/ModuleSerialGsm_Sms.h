#ifndef _ModuleSerialGsm_Sms_H
#define _ModuleSerialGsm_Sms_H

#include <ModuleSerialGsm.h>

#define MESSAGE_NUMBER_LENGTH 30
#define MESSAGE_CONTENT_LENGTH 165

class ModuleSerialGsm_Sms : protected ModuleSerialGsm 
{
public:
    ModuleSerialGsm_Sms(ModuleSerialCore *core);

    int enable(const char *pin);

    bool messageAvailable();
    void messageSend(const char *number, const char *content);
    void messageFlush();

    void receivedNumber(char *output, int size);
    void receivedContent(char *output, int size);

private:
    char messageNumber[MESSAGE_NUMBER_LENGTH];
    char messageContent[MESSAGE_CONTENT_LENGTH];

    void parseMessage(char *message);
};

#endif 
