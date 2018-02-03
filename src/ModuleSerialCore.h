#ifndef _ModuleSerialCore_H
#define _ModuleSerialCore_H

#include <Arduino.h>
#include <AltSoftSerial.h>

#define MODULE_FAIL -1
#define MODULE_READY 0

#define TIMEOUT 2000

#define BUF_LONG_LEN 200
#define BUF_MEDIUM_LEN 100
#define BUF_SHORT_LEN 50

#define DELAY_LONG 2000
#define DELAY_MEDIUM 1000
#define DELAY_SHORT 250 

class ModuleSerialCore : public AltSoftSerial
{
public:
    ModuleSerialCore(int rxPin, int txPin);
    ~ModuleSerialCore();

    void debug(HardwareSerial *printer);
    
    int begin(int baudRate);
    void end();

    bool writeCommand(const char *command, const char *expected, unsigned long timeout);
    void writeCommand(const char *command, char *output, int size, unsigned long timeout);

    int available();
    int read();
protected:
    HardwareSerial *printer = nullptr;
};

#endif 
