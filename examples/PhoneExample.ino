#include "ModuleSerialCore.h"
#include "ModuleSerialGsm_Phone.h"

#define PIN_NUMBER ""

#define LOCAL_PHONE ""
#define REMOTE_PHONE ""

#define BAUD_RATE 9600

#define CALL_TIMEOUT 10000

#define NUMBER_LEN 30
#define ALERT_LEN 50

ModuleSerialCore core(2, 3);            // Begin a SoftwareSerial connection on rx and tx pins.
ModuleSerialGsm_Phone gsmPhone(&core);  // Pass a reference to the core.

void setup() 
{
    Serial.begin(BAUD_RATE);
    while (!Serial);

    Serial.println(F("Initializing..."));

    bool notConnected = true;

    core.debug(&Serial);    // Pass a reference to HardwareSerial if you want debugging printed to the Serial Monitor.

    while (notConnected)
    {
        if (core.begin(BAUD_RATE) == MODULE_READY &&
            gsmPhone.enable(PIN_NUMBER) == GSM_ENABLED)
        {
            notConnected = false;
        }
        else 
        {
            Serial.println(F("Failed to connect."));
            delay(1000);
        }
    }

    char alert[ALERT_LEN] = "";
    sprintf(alert, "Calling %s!", REMOTE_PHONE);
    Serial.println(alert);

    gsmPhone.callMake(REMOTE_PHONE, CALL_TIMEOUT);     // Pass a phone number and timeout in milliseconds.

    Serial.println(F("Dropping call."));
    gsmPhone.callDrop();

    Serial.println(F("Ready."));

    delay(1000);
}

void loop() 
{
    if (gsmPhone.callAvailable())
    {
        char number[NUMBER_LEN] = "";
        gsmPhone.receivedNumber(number, NUMBER_LEN);

        char alert[ALERT_LEN] = "";
        sprintf(alert, "Call from %s available!", number);
        
        Serial.println(alert);

        gsmPhone.callAnswer();

        delay(10000);

        gsmPhone.callDrop();
    }

    delay(1000);
}
