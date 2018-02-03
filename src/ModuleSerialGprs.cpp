#include "ModuleSerialGprs.h"

ModuleSerialGprs::ModuleSerialGprs(ModuleSerialCore *core)
{
    this->core = core;
}

int ModuleSerialGprs::enable(const char *apn, const char* username, const char *password)
{
    if (!core->writeCommand("AT+CGATT=1", "OK", TIMEOUT))
    {
        return GPRS_FAIL;
    }
        
    core->writeCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", TIMEOUT);

    char command[BUF_SHORT_LEN] = "";

    sprintf(command, "AT+SAPBR=3,1,\"APN\",\"%s\"", apn);
    core->writeCommand(command, "OK", TIMEOUT);

    sprintf(command, "AT+SAPBR=3,1,\"USER\",\"%s\"", username);
    core->writeCommand(command, "OK", TIMEOUT);

    sprintf(command, "AT+SAPBR=3,1,\"PWD\",\"%s\"", password);
    core->writeCommand(command, "OK", TIMEOUT);

    if (core->writeCommand("AT+SAPBR=2,1", "0.0.0.0", TIMEOUT))
    {
        if (!core->writeCommand("AT+SAPBR=1,1", "OK", TIMEOUT))
        {
            return GPRS_FAIL;
        }
    }

    return GPRS_ENABLED;
}

void ModuleSerialGprs::disable()
{
    core->writeCommand("AT+CGATT=0", "+SAPBR 1: DEACT", TIMEOUT);
}

void ModuleSerialGprs::openHttpConnection()
{
    core->writeCommand("AT+HTTPINIT", "OK", TIMEOUT);
}

void ModuleSerialGprs::closeHttpConnection()
{
    core->writeCommand("AT+HTTPTERM", "OK", TIMEOUT);
}

ModuleSerialGprs::HttpResponse ModuleSerialGprs::sendHttpRequest(int method, const char *url, unsigned long timeout)
{
    // core->writeCommand("AT+HTTPSSL=1", "OK", TIMEOUT);
    core->writeCommand("AT+HTTPPARA=\"CID\",1", "OK", TIMEOUT);

    char command[BUF_LONG_LEN] = "";
    sprintf(command, "AT+HTTPPARA=\"URL\",\"%s\"", url);

    core->writeCommand(command, "OK", TIMEOUT);

    char response[BUF_SHORT_LEN] = "";
    sprintf(command, "AT+HTTPACTION=%d", method);
    
    core->writeCommand(command, response, BUF_SHORT_LEN, timeout);

    ModuleSerialGprs::HttpResponse httpResponse;

    if (strstr(response, "+HTTPACTION:") != NULL)
    {
        parseHttpResponse(&httpResponse, response);
    }

    return httpResponse;
}

void ModuleSerialGprs::readHttpResponse(int count, char *output, int size)
{
    char command[BUF_SHORT_LEN] = "";
    sprintf(command, "AT+HTTPREAD=0,%d", count);

    core->writeCommand(command, output, size, TIMEOUT);

    if (strstr(output, "+HTTPREAD:") != NULL)
    {
        parseReadContent(output);
    }
}

void ModuleSerialGprs::parseHttpResponse(ModuleSerialGprs::HttpResponse *httpResponse, char *response)
{
    char *pch;

    pch = strtok(response, " ,");
    pch = strtok(NULL, " ,");
    httpResponse->method = atoi(pch);

    pch = strtok(NULL, " ,");
    httpResponse->statusCode = atoi(pch);

    pch = strtok(NULL, " ,");
    httpResponse->contentLength = atoi(pch);
}

void ModuleSerialGprs::parseReadContent(char *content)
{
    char *pch;

    pch = strtok(content, "\n");
    pch = strtok(NULL, "\n");
    pch = strtok(NULL, "\n");
    
    strcpy(content, pch);
}
