#ifndef _ModuleSerialGsm_Phone_H
#define _ModuleSerialGsm_Phone_H

#define CALL_NUMBER_LENGTH 30

#include "ModuleSerialGsm.h"

class ModuleSerialGsm_Phone : public ModuleSerialGsm 
{
public:
    ModuleSerialGsm_Phone(ModuleSerialCore *core);

    int enable(const char *pin);

    bool callAvailable();
    void callMake(const char *number, unsigned long timeout);
    void callAnswer();
    void callDrop();

    void receivedNumber(char *output, int size);

private:
    char callNumber[CALL_NUMBER_LENGTH];

    void parseCall(char *call);
};

#endif 
