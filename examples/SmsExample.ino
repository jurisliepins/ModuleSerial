#include <ModuleSerialCore.h>
#include <ModuleSerialGsm_Sms.h>

#define PIN_NUMBER ""

#define LOCAL_PHONE ""
#define REMOTE_PHONE ""

ModuleSerialCore core(2, 3);		// Begin a SoftwareSerial connection on rx and tx pins.
ModuleSerialGsm_Sms gsmSms(&core);	// Pass a reference to the core.

void setup() 
{
	Serial.begin(9600);
	while (!Serial);

	Serial.println(F("Initializing..."));

	bool notConnected = true;

	while (notConnected)
	{
		core.debug(&Serial);	// Pass a reference to HardwareSerial if you want debugging printed to the Serial Monitor.

		if (core.begin(9600) == MODULE_READY &&
			gsmSms.enable(PIN_NUMBER) == GSM_ENABLED)
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
	char network[15] = "";
	gsmSms.currentNetwork(network);

	char message[50] = "";
	sprintf(message, "%d %d %d, %d, %s", 
		batteryStatus.mode, batteryStatus.capacity, batteryStatus.voltage, 
		signalQuality, network);

	Serial.println(F("Sending bootup message."));

	gsmSms.messageSend(REMOTE_PHONE, message);
	gsmSms.messageFlush();		// Delete 'read', 'sent' and 'saved but unsent' messages.

	Serial.println(F("Ready."));

	delay(1000);
}

void loop() 
{
	if (gsmSms.messageAvailable())
	{
		char receivedNumber[30] = "";
		gsmSms.receivedNumber(receivedNumber, 30);	// Phone number of the sender.

		Serial.println(F("Message received from: "));
		Serial.println(receivedNumber);

		char receivedContent[165] = "";
		gsmSms.receivedContent(receivedContent, 165);	// Contents of the SMS.

		Serial.println(F("Message contains: "));
		Serial.println(receivedContent);

		gsmSms.messageFlush();	// Delete 'read', 'sent' and 'saved but unsent' messages.
	}

	delay(1000);
}
