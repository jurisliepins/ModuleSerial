#include <ModuleSerialGprs.h>

ModuleSerialGprs::ModuleSerialGprs(ModuleSerialCore *core)
{
    this->core = core;
}

int ModuleSerialGprs::enable(const char *apn, const char* username, const char *password)
{
    if (!core->writeCommand("AT+CGATT=1", "OK", 2000))
        return GPRS_FAIL;

    core->writeCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", 2000);

    char command[50] = "";

    sprintf(command, "AT+SAPBR=3,1,\"APN\",\"%s\"", apn);
    core->writeCommand(command, "OK", 2000);

    sprintf(command, "AT+SAPBR=3,1,\"USER\",\"%s\"", username);
    core->writeCommand(command, "OK", 2000);

    sprintf(command, "AT+SAPBR=3,1,\"PWD\",\"%s\"", password);
    core->writeCommand(command, "OK", 2000);

    if (core->writeCommand("AT+SAPBR=2,1", "0.0.0.0", 2000))
    {
        if (!core->writeCommand("AT+SAPBR=1,1", "OK", 2000))
            return GPRS_FAIL;   
    }

    return GPRS_ENABLED;
}

void ModuleSerialGprs::disable()
{
    core->writeCommand("AT+CGATT=0", "+SAPBR 1: DEACT", 2000);
}

void ModuleSerialGprs::openHttpConnection()
{
    core->writeCommand("AT+HTTPINIT", "OK", 2000);
}

void ModuleSerialGprs::closeHttpConnection()
{
    core->writeCommand("AT+HTTPTERM", "OK", 2000);
}

ModuleSerialGprs::HttpResponse ModuleSerialGprs::sendHttpRequest(int method, const char *url, unsigned long timeout)
{
    // core->writeCommand("AT+HTTPSSL=1", "OK", 2000);
    core->writeCommand("AT+HTTPPARA=\"CID\",1", "OK", 2000);

    char command[200] = "";
    sprintf(command, "AT+HTTPPARA=\"URL\",\"%s\"", url);

    core->writeCommand(command, "OK", 2000);

    char response[50] = "";
    sprintf(command, "AT+HTTPACTION=%d", method);
    core->writeCommand(command, response, 50, timeout);

    ModuleSerialGprs::HttpResponse httpResponse;

    if (strstr(response, "+HTTPACTION:") != NULL)
    {
        parseHttpResponse(&httpResponse, response);
    }

    return httpResponse;
}

void ModuleSerialGprs::readHttpResponse(int count, char *output, int size)
{
    char command[50] = "";
    sprintf(command, "AT+HTTPREAD=0,%d", count);

    core->writeCommand(command, output, size, 2000);

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
