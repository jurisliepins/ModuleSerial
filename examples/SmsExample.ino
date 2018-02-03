#include "ModuleSerialCore.h"
#include "ModuleSerialGsm_Sms.h"

#define PIN_NUMBER ""

#define LOCAL_PHONE ""
#define REMOTE_PHONE ""

#define BAUD_RATE 9600

#define NETWORK_LEN 15
#define MESSAGE_LEN 50
#define RECEIVED_NUMBER_LEN 30
#define RECEIVED_CONTENT_LEN 165

ModuleSerialCore core(2, 3);        // Begin a serial connection on rx and tx pins.
ModuleSerialGsm_Sms gsmSms(&core);  // Pass a reference to the core.

void setup() 
{
    Serial.begin(BAUD_RATE);
    while (!Serial);

    Serial.println(F("Initializing..."));

    core.debug(&Serial);            // Pass a reference to HardwareSerial if you want debugging printed to the Serial Monitor.

    bool notConnected = true;

    while (notConnected)
    {
        if (core.begin(BAUD_RATE) == MODULE_READY && gsmSms.enable(PIN_NUMBER) == GSM_ENABLED)
        {
            notConnected = false;
        }
        else 
        {
            Serial.println(F("Failed to connect."));
            delay(1000);
        }
    }

    // Battery status, signal quality and current network can be accessed from ModuleSerialGsm_Phone.h as well.
    ModuleSerialGsm::BatteryStatus batteryStatus = gsmSms.currentBatteryStatus();
    int signalQuality = gsmSms.currentSignalQuality();
    char network[NETWORK_LEN] = "";
    gsmSms.currentNetwork(network);

    char message[MESSAGE_LEN] = "";
    sprintf(message, "%d %d %d, %d, %s", 
        batteryStatus.mode, batteryStatus.capacity, batteryStatus.voltage, 
        signalQuality, network);

    Serial.println(F("Sending bootup message."));

    gsmSms.messageSend(REMOTE_PHONE, message);
    gsmSms.messageFlush();      // Delete 'read', 'sent' and 'saved but unsent' messages.

    Serial.println(F("Ready."));

    delay(1000);
}

void loop() 
{
    if (gsmSms.messageAvailable())
    {
        char receivedNumber[RECEIVED_NUMBER_LEN] = "";
        gsmSms.receivedNumber(receivedNumber, RECEIVED_NUMBER_LEN);  // Phone number of the sender.

        Serial.println(F("Message received from: "));
        Serial.println(receivedNumber);

        char receivedContent[RECEIVED_CONTENT_LEN] = "";
        gsmSms.receivedContent(receivedContent, RECEIVED_CONTENT_LEN);   // Contents of the SMS.

        Serial.println(F("Message contains: "));
        Serial.println(receivedContent);

        gsmSms.messageFlush();  // Delete 'read', 'sent' and 'saved but unsent' messages.
    }

    delay(1000);
}
