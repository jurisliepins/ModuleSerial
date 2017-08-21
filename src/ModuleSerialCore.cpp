#include <ModuleSerialCore.h>

ModuleSerialCore::ModuleSerialCore(int rxPin, int txPin) : SoftwareSerial(rxPin, txPin)
{
}

void ModuleSerialCore::debug(HardwareSerial *printer)
{
    this->printer = printer;
}

int ModuleSerialCore::begin(int baudRate)
{
    SoftwareSerial::begin(baudRate);

    if (!writeCommand("AT", "OK", 2000))
        return MODULE_FAIL;

    if (printer != nullptr)
    {
        writeCommand("AT+CMEE=1", "OK", 2000); // Set verbose error reporting.
    }

    return MODULE_READY;
}

bool ModuleSerialCore::writeCommand(const char *command, const char *expected, unsigned long timeout)
{
    SoftwareSerial::flush();
    SoftwareSerial::println(command);

    char response[200] = "";
    int i = 0;

    unsigned long start = millis();

    do 
    {
        while (SoftwareSerial::available())
        {
            response[i++] = SoftwareSerial::read();
            if (i >= 199) 
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

    int i = 0;
    
    unsigned long start = millis();

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