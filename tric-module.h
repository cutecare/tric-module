/*
Part of http://cutecare.ru project
Author: evgeny.savitsky@gmail.com
*/

#ifndef TRICModule_h
#define TRICModule_h

#if (ARDUINO >= 100)
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif
#include <SoftwareSerial.h>

class TRIC {
public:
	static const int LED_R = 6;
	static const int LED_G = 13;
	static const int LED_B = 10;
	static const int BAUD = 4800;

  public:
	  TRIC(int rxPin = 12, int txPin = 8, int resetPin = PB0);
	  bool configure(const char * bleName, int io1 = 0, int io2 = 0, int io3 = 0, int io4 = 0);
	  void setData(unsigned int minor = 0, unsigned int major = 0, short humidity = 0, short temperature = 0, short battery = 0);
	  void major(unsigned int value);
	  void minor(unsigned int value);
	  void switchState(bool state);
	  void humidity(short value);
	  void temperature(short value);
	  void battery(short value);
	  void useRemoteXY();
	  void powerOn();
	  void powerOff();
	  void rgb(int r, int g, int b);

  private:
	  bool sendCommand(const SoftwareSerial * bleSerial, const char * data);
	  void rgbDelay(int r, int g, int b, int delay);

	  int bleTxPin = 0;
	  int bleRxPin = 0;
	  int bleResetPin = 0;
	  int remoteXYMode = 0;
};

#endif
