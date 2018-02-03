#include "SoftwareSerial/ModuleSerialCore.h"
#include "SoftwareSerial/ModuleSerialGps.h"

#define PIN_NUMBER ""

#define LOCAL_PHONE ""
#define REMOTE_PHONE ""

#define BAUD_RATE 9600

ModuleSerialCore core(2, 3);    // Begin a SoftwareSerial connection on rx and tx pins.
ModuleSerialGps gps(&core);     // Pass a reference to the core.

void setup() 
{
    Serial.begin(BAUD_RATE);
    while (!Serial);

    Serial.println(F("Initializing..."));

    core.debug(&Serial);        // Pass a reference to HardwareSerial if you want debugging printed to the Serial Monitor.

    bool notConnected = true;

    while (notConnected)
    {
        if (core.begin(BAUD_RATE) == MODULE_READY &&
            gps.enable() == GPS_ENABLED)
        {
            notConnected = false;
        }
        else 
        {
            Serial.println(F("Failed to connect."));
            delay(1000);
        }
    }

    delay(1000);
}

void loop() 
{
    ModuleSerialGps::GpsData gpsData = gps.currentGpsData();

    Serial.println(F("\nCurrent GPS data: "));

    Serial.println(gpsData.mode);
    Serial.println(gpsData.lng);
    Serial.println(gpsData.lat);
    Serial.println(gpsData.alt);
    Serial.println(gpsData.UTCTime);
    Serial.println(gpsData.TTFF);
    Serial.println(gpsData.sat);
    Serial.println(gpsData.speed);
    Serial.println(gpsData.course);

    delay(1000);
}
