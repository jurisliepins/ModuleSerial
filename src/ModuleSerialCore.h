#ifndef _ModuleSerialCore_H
#define _ModuleSerialCore_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#define MODULE_FAIL -1
#define MODULE_READY 0

class ModuleSerialCore : public SoftwareSerial 
{
public:
    ModuleSerialCore(int rxPin, int txPin);

    void debug(HardwareSerial *printer);
    int begin(int baudRate);

    bool writeCommand(const char *command, const char *expected, unsigned long timeout);
    void writeCommand(const char *command, char *output, int size, unsigned long timeout);
protected:
    HardwareSerial *printer = nullptr;
};

#endif 
