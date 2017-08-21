#include <ModuleSerialGps.h>

ModuleSerialGps::ModuleSerialGps(ModuleSerialCore *core)
{
    this->core = core;
}

int ModuleSerialGps::enable()
{
    if (!core->writeCommand("AT+CGPSPWR=1", "OK", 2000))
        return GPS_FAIL;
    if (!core->writeCommand("AT+CGPSRST=0", "OK", 2000))
        return GPS_FAIL;

    return GPS_ENABLED;
}

void ModuleSerialGps::disable()
{
    core->writeCommand("AT+CGPSPWR=0", "OK", 2000);
}

ModuleSerialGps::GpsData ModuleSerialGps::currentGpsData()
{
    char response[200] = "";
    core->writeCommand("AT+CGPSINF=0", response, 200, 2000);

    ModuleSerialGps::GpsData GpsData;

    if (strstr(response, "+CGPSINF:") != NULL)
    {
        parseGpsData(&GpsData, response);
    }

    return GpsData;
}

void ModuleSerialGps::parseGpsData(ModuleSerialGps::GpsData *GpsData, char *data)
{
    char *pch;
    pch = strtok(data, " ,");

    pch = strtok(NULL, " ,");
    GpsData->mode = atoi(pch);

    pch = strtok(NULL, " ,");
    GpsData->lng = atof(pch);

    pch = strtok(NULL, " ,");
    GpsData->lat = atof(pch);

    pch = strtok(NULL, " ,");
    GpsData->alt = atof(pch);

    pch = strtok(NULL, " ,");
    strcpy(GpsData->UTCTime, pch);
    
    pch = strtok(NULL, " ,");
    strcpy(GpsData->TTFF, pch);

    pch = strtok(NULL, " ,");
    GpsData->sat = atoi(pch);

    pch = strtok(NULL, " ,");
    GpsData->speed = atof(pch);
                
    pch = strtok(NULL, " ,");               
    GpsData->course = atof(pch);                
}
