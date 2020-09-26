/*
Part of http://cutecare.ru project
Author: evgeny.savitsky@gmail.com
*/
#include "tric-module.h"

/**
 * Constructor
 */
TRIC::TRIC(int rxPin, int txPin, int resetPin)
{
	bleRxPin = rxPin;
	bleTxPin = txPin;
	bleResetPin = resetPin;
}

bool TRIC::configure(const char * bleName, int io1 = 0, int io2 = 0, int io3 = 0, int io4 = 0)
{
	powerOn();

	int result = false;
	SoftwareSerial bleSerialInitial(bleTxPin, bleRxPin);
	int bauds[] = {2400, 4800, 9600, 19200, 38400, 57600, 115200};
	for( int i = 0; i < sizeof(bauds) / sizeof(int); i++ ) {
		bleSerialInitial.begin(bauds[i]);
		sendCommand(&bleSerialInitial, "AT+RST");
		result = sendCommand(&bleSerialInitial, "AT+RESTORE");
		rgbDelay(233, 19, 40, 1000);
		if ( result ) break;
	}
	if ( !result) {
		rgb(255, 0, 0);
		return false;
	}
	
	sendCommand(&bleSerialInitial, "AT+BOUD5"); //9600
	SoftwareSerial bleSerial(bleTxPin, bleRxPin);
	bleSerial.begin(TRIC::BAUD);

	char text[64] = "";
	sprintf(text, "AT+NAME%s", bleName);
	sendCommand(&bleSerial, text);

	if ( remoteXYMode == 0 ) {
		sendCommand(&bleSerial, "AT+HOSTEN3");
	}
	else {
		sendCommand(&bleSerial, "AT+HOSTEN0");
	}
	sendCommand(&bleSerial, "AT+CLSSE0");
	sendCommand(&bleSerial, "AT+RST");
	sendCommand(&bleSerial, "AT+POWR0");      // max RF power
	sendCommand(&bleSerial, "AT+ADVEN1");     // advertising enabled
	sendCommand(&bleSerial, "AT+ADVIN1");     // setup advertising interval
	
	sprintf(text, "AT+PIO1%d", io1);
	sendCommand(&bleSerial, text);
	sprintf(text, "AT+PIO2%d", io2);
	sendCommand(&bleSerial, text);
	sprintf(text, "AT+PIO3%d", io3);
	sendCommand(&bleSerial, text);
	sprintf(text, "AT+PIO4%d", io4);
	sendCommand(&bleSerial, text);

	pinMode(bleRxPin, INPUT);
	return true;
}

void TRIC::setData(unsigned int minor = 0, unsigned int major = 0, short humidity = 0, short temperature = 0, short battery = 0)
{
	pinMode(bleRxPin, OUTPUT);

	SoftwareSerial bleSerial(bleTxPin, bleRxPin);
	bleSerial.begin(TRIC::BAUD);

	char buff[32] = "";
	sprintf(buff, "AT+MAJOR%04X", major);
	sendCommand(&bleSerial, buff);
	sprintf(buff, "AT+MINOR%04X", minor);
	sendCommand(&bleSerial, buff);
	sprintf(buff, "AT+HUMID%02X", humidity);
	sendCommand(&bleSerial, buff);
	sprintf(buff, "AT+TEMP%02X", temperature);
	sendCommand(&bleSerial, buff);
	sprintf(buff, "AT+BATT%02X", battery);
	sendCommand(&bleSerial, buff);

	pinMode(bleRxPin, INPUT);
}

void TRIC::major(unsigned int value)
{
	pinMode(bleRxPin, OUTPUT);

	SoftwareSerial bleSerial(bleTxPin, bleRxPin);
	bleSerial.begin(TRIC::BAUD);

	char buff[32] = "";
	sprintf(buff, "AT+MAJOR%04X", value);
	sendCommand(&bleSerial, buff);

	pinMode(bleRxPin, INPUT);
}

void TRIC::minor(unsigned int value)
{
	pinMode(bleRxPin, OUTPUT);

	SoftwareSerial bleSerial(bleTxPin, bleRxPin);
	bleSerial.begin(TRIC::BAUD);

	char buff[32] = "";
	sprintf(buff, "AT+MINOR%04X", value);
	sendCommand(&bleSerial, buff);

	pinMode(bleRxPin, INPUT);
}

void TRIC::humidity(short value)
{
	pinMode(bleRxPin, OUTPUT);

	SoftwareSerial bleSerial(bleTxPin, bleRxPin);
	bleSerial.begin(TRIC::BAUD);

	char buff[32] = "";
	sprintf(buff, "AT+HUMID%02X", value);
	sendCommand(&bleSerial, buff);

	pinMode(bleRxPin, INPUT);
}

void TRIC::temperature(short value)
{
	pinMode(bleRxPin, OUTPUT);

	SoftwareSerial bleSerial(bleTxPin, bleRxPin);
	bleSerial.begin(TRIC::BAUD);

	char buff[32] = "";
	sprintf(buff, "AT+TEMP%02X", value);
	sendCommand(&bleSerial, buff);

	pinMode(bleRxPin, INPUT);
}

void TRIC::battery(short value)
{
	pinMode(bleRxPin, OUTPUT);

	SoftwareSerial bleSerial(bleTxPin, bleRxPin);
	bleSerial.begin(TRIC::BAUD);

	char buff[32] = "";
	sprintf(buff, "AT+BATT%02X", value);
	sendCommand(&bleSerial, buff);

	pinMode(bleRxPin, INPUT);
}

bool TRIC::sendCommand(const SoftwareSerial * bleSerial, const char * data) {
	rgbDelay(53, 4, 230, 250);
	bleSerial->print(data);
	bool result = false;
	char c;
	do {
		c = bleSerial->read();
		if ( c > 0 ) rgbDelay(0, c, 0, 10);
		if ( c == 79 ) result = true;
	} while( c > 0 );
	rgb(0, 0, 0);
	return result;
}

void TRIC::useRemoteXY() {
	remoteXYMode = 1;
}

void TRIC::powerOn() 
{
  pinMode(PD5, OUTPUT);
  digitalWrite(PD5, HIGH);
  pinMode(bleRxPin, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  rgbDelay(231, 68, 206, 1000);
}

void TRIC::powerOff() 
{
  pinMode(PD5, OUTPUT);
  digitalWrite(PD5, LOW);
}

void TRIC::rgb(int r, int g, int b)
{
	analogWrite(LED_R, 255 - r);
	analogWrite(LED_G, 255 - g);
	analogWrite(LED_B, 255 - b);
}

void TRIC::switchState(bool state)
{
	major(state ? 255 : 0);
}

void TRIC::rgbDelay(int r, int g, int b, int timeout)
{
  rgb(r, g, b);
  delay(timeout);
  rgb(0, 0, 0);
}
