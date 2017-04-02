#include <ModuleSerialCore.h>
#include <ModuleSerialGprs.h>

#define PIN_NUMBER ""

#define APN ""
#define APN_USERNAME ""
#define APN_PASSWORD ""

#define LOCAL_PHONE ""
#define REMOTE_PHONE ""

ModuleSerialCore core(2, 3);
ModuleSerialGprs gprs(&core);

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
			gprs.enable(APN, APN_USERNAME, APN_PASSWORD) == GPRS_ENABLED)
		{
			notConnected = false;
		}
		else 
		{
			Serial.println(F("Failed to connect."));
			delay(1000);
		}
	}

	gprs.openHttpConnection();
	ModuleSerialGprs::HttpResponse httpResponse = 
		gprs.sendHttpRequest(HTTP_GET, "httpbin.org/get?param=1&param=2", 6000);

	if (httpResponse.statusCode == 200)
	{
		Serial.println(F("HTTP_GET response success!"));

		char response[500] = "";
		int length = httpResponse.contentLength + 50 > 500 ? 
			450 : httpResponse.contentLength;

		gprs.readHttpResponse(length, response, 500);

		Serial.println(response);
	}
	else
	{
		Serial.println(F("HTTP failed."));
	}

	gprs.closeHttpConnection();

	delay(1000);
}

void loop() 
{
	gprs.openHttpConnection();
	ModuleSerialGprs::HttpResponse httpResponse = 
		gprs.sendHttpRequest(HTTP_POST, "httpbin.org/post?param=1&param=2", 6000);

	if (httpResponse.statusCode == 200)
	{
		Serial.println(F("HTTP response success!"));

		char response[500] = "";
		int length = httpResponse.contentLength + 50 > 500 ? 
			450 : httpResponse.contentLength;

		gprs.readHttpResponse(length, response, 500);

		Serial.println(response);
	}
	else
	{
		Serial.println(F("HTTP failed."));
	}

	gprs.closeHttpConnection();	

	delay(1000);
}