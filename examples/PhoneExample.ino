#include <ModuleSerialCore.h>
#include <ModuleSerialGsm_Phone.h>

#define PIN_NUMBER ""

#define LOCAL_PHONE ""
#define REMOTE_PHONE ""

ModuleSerialCore core(2, 3);
ModuleSerialGsm_Phone gsmPhone(&core);

void setup() 
{
	Serial.begin(9600);
	while (!Serial);

	Serial.println(F("Initializing..."));

	bool notConnected = true;

	while (notConnected)
	{
		core.debug(&Serial);

		if (core.begin(9600) == MODULE_READY &&
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

	char alert[50] = "";
	sprintf(alert, "Calling %s!", REMOTE_PHONE);
	Serial.println(alert);

	gsmPhone.callMake(REMOTE_PHONE, 10000);

	Serial.println(F("Dropping call."));
	gsmPhone.callDrop();

	Serial.println(F("Ready."));

	delay(1000);
}

void loop() 
{
	if (gsmPhone.callAvailable())
	{
		char number[30] = "";
		gsmPhone.receivedNumber(number, 30);

		char alert[50] = "";
		sprintf(alert, "Call from %s available!", number);
		
		Serial.println(alert);

		gsmPhone.callAnswer();

		delay(10000);

		gsmPhone.callDrop();
	}

	delay(1000);
}
