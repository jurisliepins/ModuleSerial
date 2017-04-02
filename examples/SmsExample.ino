#include <ModuleSerialCore.h>
#include <ModuleSerialGsm_Sms.h>

#define PIN_NUMBER ""

#define LOCAL_PHONE ""
#define REMOTE_PHONE ""

ModuleSerialCore core(2, 3);
ModuleSerialGsm_Sms gsmSms(&core);

void setup() 
{
	Serial.begin(9600);
	while (!Serial);

	Serial.println(F("Initializing..."));

	bool notConnected = true;

	while (notConnected)
	{
		// core.debug(&Serial);

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
	gsmSms.messageFlush();

	Serial.println(F("Ready."));

	delay(1000);
}

void loop() 
{
	if (gsmSms.messageAvailable()) 
	{
		char receivedNumber[30] = "";
		gsmSms.receivedNumber(receivedNumber, 30);

		Serial.println(F("Message received from: "));
		Serial.println(receivedNumber);

		char receivedContent[165] = "";
		gsmSms.receivedContent(receivedContent, 165);

		Serial.println(F("Message contains: "));
		Serial.println(receivedContent);

		gsmSms.messageFlush();	
	}

	delay(1000);
}
