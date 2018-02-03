#include "ModuleSerialCore.h"

ModuleSerialCore::ModuleSerialCore(int rxPin, int txPin) : AltSoftSerial::AltSoftSerial(rxPin, txPin)
{
}

ModuleSerialCore::~ModuleSerialCore()
{
    AltSoftSerial::end();
}

void ModuleSerialCore::debug(HardwareSerial *printer)
{
    this->printer = printer;
}

int ModuleSerialCore::begin(int baudRate)
{
    AltSoftSerial::begin(baudRate);

    if (!writeCommand("AT", "OK", TIMEOUT))
    {
        return MODULE_FAIL;    
    }

    if (printer != nullptr)
    {
        writeCommand("AT+CMEE=1", "OK", TIMEOUT); // Set verbose error reporting.
    }

    return MODULE_READY;
}

void ModuleSerialCore::end()
{
    AltSoftSerial::end();
}

bool ModuleSerialCore::writeCommand(const char *command, const char *expected, unsigned long timeout)
{
    AltSoftSerial::flush();
    AltSoftSerial::println(command);

    char response[BUF_LONG_LEN] = "";
    int i = 0;

    unsigned long start = millis();

    do 
    {
        while (AltSoftSerial::available())
        {
            response[i++] = AltSoftSerial::read();

            if (i >= BUF_LONG_LEN - 1) 
            {
                if (printer != nullptr)
                {
                    printer->println(F("Buffer overflow!"));
                }

                response[i] = '\0';

                return false;
            }
        }
    } while ((millis() - start) < timeout);

    response[i] = '\0';

    if (printer != nullptr)
    {
        printer->println(response);
    }

    return strstr(response, expected) != NULL;
}

void ModuleSerialCore::writeCommand(const char *command, char *output, int size, unsigned long timeout)
{
    AltSoftSerial::flush();
    AltSoftSerial::println(command);

    unsigned long start = millis();
    int i = 0;

    do 
    {
        while (AltSoftSerial::available())
        {
            output[i++] = AltSoftSerial::read();

            if (i >= size - 1) 
            {
                if (printer != nullptr)
                {
                    printer->println(F("Buffer overflow!"));
                }

                output[i] = '\0';
                
                return;
            }
        }
    } while ((millis() - start) < timeout);

    output[i] = '\0';

    if (printer != nullptr)
    {
        printer->println(output);
    }
}

int ModuleSerialCore::available()
{
    return AltSoftSerial::available();
}

int ModuleSerialCore::read()
{
    return AltSoftSerial::read();
}