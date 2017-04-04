# ModuleSerial
GSM/GPS/GPRS library for Arduino.

ModuleSerial provides an interface for sending GSM/GPS/GPRS AT commands to a GSM shield using SoftwareSerial. It's a lighweight alternative to the existing GSM libraries. If memory is not an issue, you should probably use [GSM Library for Arduino](https://github.com/arduino/Arduino/tree/master/libraries/GSM) instead. The library has been tested only on ATmega328 with a SIM808 shield. 

Complete examples are in the "examples" folder.

# 1. Initialization 

```
ModuleSerialCore core(2, 3);		// Begin a SoftwareSerial connection on rx and tx pins.
ModuleSerialGsm_Sms gsmSms(&core);	
ModuleSerialGsm_Phone gsmPhone(&core);
ModuleSerialGps gps(&core);
ModuleSerialGprs gprs(&core);
```

# 2. Setup

```
Serial.begin(9600);
while (!Serial);

Serial.println(F("Initializing..."));

bool notConnected = true;

while (notConnected)
{
  core.debug(&Serial);	// Pass a reference to HardwareSerial if you want debugging printed to the Serial Monitor.

  if (core.begin(9600) == MODULE_READY &&
      gsmSms.enable(PIN_NUMBER) == GSM_ENABLED &&
      gsmPhone.enable(PIN_NUMBER) == GSM_ENABLED &&
      gps.enable() == GPS_ENABLED &&
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
```
  
# 3. Sending and receiving SMS
  
```
gsmSms.messageSend(PHONE_NUMBER, message);

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

  gsmSms.messageFlush();	// Delete 'read', 'sent' and 'saved but unsent' messages.
}
```

# 4. Calling

```
gsmPhone.callMake(PHONE_NUMBER, 10000);		// Pass a phone number and timeout in milliseconds.

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

```

# 5. Location

```
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
```

# 6. HTTP requests

```
gprs.openHttpConnection();
ModuleSerialGprs::HttpResponse httpResponse = 
  gprs.sendHttpRequest(HTTP_GET, "httpbin.org/get?param=1&param=2", 6000);	// Method, url and timeout in milliseconds.

if (httpResponse.statusCode == 200)
{
  Serial.println(F("Request successful!"));

  char response[500] = "";
  int length = httpResponse.contentLength + 50 > 500 ? 450 : httpResponse.contentLength;  // Read the first 450 characters of the response.

  gprs.readHttpResponse(length, response, 500);

  Serial.println(response);
}
else
{
  Serial.println(F("Request failed."));
}

gprs.closeHttpConnection();
```
