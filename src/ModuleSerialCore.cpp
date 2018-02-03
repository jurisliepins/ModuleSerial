#include "ModuleSerialCore.h"

ModuleSerialCore::ModuleSerialCore(int rxPin, int txPin) : SoftwareSerial::SoftwareSerial(rxPin, txPin)
{
}

ModuleSerialCore::~ModuleSerialCore()
{
    SoftwareSerial::end();
}

void ModuleSerialCore::debug(HardwareSerial *printer)
{
    this->printer = printer;
}

int ModuleSerialCore::begin(int baudRate)
{
    SoftwareSerial::begin(baudRate);

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
    SoftwareSerial::end();
}

bool ModuleSerialCore::writeCommand(const char *command, const char *expected, unsigned long timeout)
{
    SoftwareSerial::flush();
    SoftwareSerial::println(command);

    char response[BUF_LONG_LEN] = "";
    int i = 0;

    unsigned long start = millis();

    do 
    {
        while (SoftwareSerial::available())
        {
            response[i++] = SoftwareSerial::read();

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
    SoftwareSerial::flush();
    SoftwareSerial::println(command);

    unsigned long start = millis();
    int i = 0;

    do 
    {
        while (SoftwareSerial::available())
        {
            output[i++] = SoftwareSerial::read();

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
    return SoftwareSerial::available();
}

int ModuleSerialCore::read()
{
    return SoftwareSerial::read();
}