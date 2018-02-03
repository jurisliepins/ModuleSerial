#ifndef _ModuleSerialGprs_H
#define _ModuleSerialGprs_H

#include "ModuleSerialCore.h"

#define GPRS_FAIL -1 
#define GPRS_ENABLED 0

#define HTTP_GET 0
#define HTTP_POST 1
#define HTTP_HEAD 2

class ModuleSerialGprs
{
public:
    struct HttpResponse
    {
        int method;
        int statusCode;
        int contentLength;
    };

    ModuleSerialGprs(ModuleSerialCore *core);

    int enable(const char *apn, const char* username, const char *password);
    void disable();

    void openHttpConnection();
    void closeHttpConnection();

    HttpResponse sendHttpRequest(int method, const char *url, unsigned long timeout);   
    void readHttpResponse(int count, char *output, int size);
private:
    ModuleSerialCore *core = nullptr;

    void parseHttpResponse(HttpResponse *httpResponse, char *response);
    void parseReadContent(char *content);
};

#endif 
