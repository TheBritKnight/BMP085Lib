/////////////////////////////////////////////////////////////////
//                      BMP085 Library                         //
//           Written by Charles Dawson, June 2013              //
//              Released under the MIT License                 //
/////////////////////////////////////////////////////////////////

//Special thanks to the folks at bildr.org and sparkfun.com (particularly Jim Lindblom).
	//Most of this code is from their tutorial, 
	 //just wrapped up nicely in library form

/*
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "Arduino.h"
#include "BMP085.h"

#define BMP085_ADDRESS 0x77 //I2C address for the BMP085 temp/pressure sensor

BMP085::BMP085(unsigned char OSS){
	//Set oversampling setting
	_OSS = OSS;	
}

void BMP085::calibrate(){
	//Retrieve factory calibration values from the sensor
	_ac1 = readInt(0xAA);
	_ac2 = readInt(0xAC);
	_ac3 = readInt(0xAE);
	_ac4 = readInt(0xB0);
	_ac5 = readInt(0xB2);
	_ac6 = readInt(0xB4);
	_b1 = readInt(0xB6);
	_b2 = readInt(0xB8);
	_mb = readInt(0xBA);
	_mc = readInt(0xBC);
	_md = readInt(0xBE);

	//And then call getTemperature() to set b5
	getTemperature();
}

//Returns temperature in degrees Celcius
float BMP085::getTemperature(){
	//First get raw temperature data
	unsigned int rawTemp = getRawTemp();

	//And then adjust it using calibration values
	long x1, x2;
	x1 = (((long)rawTemp - (long)_ac6)*(long)_ac5) >> 15;
	x2 = ((long)_mc << 11)/(x1 + _md);
	_b5 = x1 + x2;

	float temp = ((_b5 + 8)>>4);
	temp = temp /10;

	return temp;
}

//Returns pressure in atm
float BMP085::getPressure(){
	//First get raw pressure
	unsigned int rawPressure = getRawPressure();

	//And then adjust it w/ calibration values
	long x1, x2, x3, b3, b6, p;
	unsigned long b4, b7;

	b6 = _b5 - 4000;
	// Calculate B3
	x1 = (_b2 * (b6 * b6)>>12)>>11;
	x2 = (_ac2 * b6)>>11;
	x3 = x1 + x2;
	b3 = (((((long)_ac1)*4 + x3)<<_OSS) + 2)>>2;

	// Calculate B4
	x1 = (_ac3 * b6)>>13;
	x2 = (_b1 * ((b6 * b6)>>12))>>16;
	x3 = ((x1 + x2) + 2)>>2;
	b4 = (_ac4 * (unsigned long)(x3 + 32768))>>15;

	b7 = ((unsigned long)(rawPressure - b3) * (50000>>_OSS));
	if (b7 < 0x80000000)
	  p = (b7<<1)/b4;
	else
	  p = (b7/b4)<<1;

	x1 = (p>>8) * (p>>8);
	x1 = (x1 * 3038)>>16;
	x2 = (-7357 * p)>>16;
	p += (x1 + x2 + 3791)>>4;

	float temp = ((float)p)/101325.0;
	return temp;
}

unsigned int BMP085::getRawTemp(){
	unsigned int ut;

	// Write 0x2E into Register 0xF4
	// This requests a temperature reading
	Wire.beginTransmission(BMP085_ADDRESS);
	Wire.write(0xF4);
	Wire.write(0x2E);
	Wire.endTransmission();

	// Wait at least 4.5ms
	delay(5);

	// Read two bytes from registers 0xF6 and 0xF7
	ut = readInt(0xF6);
	return ut;
}

unsigned long BMP085::getRawPressure(){
	unsigned char msb, lsb, xlsb;
	unsigned long up = 0;

	// Write 0x34+(OSS<<6) into register 0xF4
	// Request a pressure reading w/ oversampling setting
	Wire.beginTransmission(BMP085_ADDRESS);
	Wire.write(0xF4);
	Wire.write(0x34 + (_OSS<<6));
	Wire.endTransmission();

	// Wait for conversion, delay time dependent on OSS
	delay(2 + (3<<_OSS));

	// Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
	msb = read(0xF6);
	lsb = read(0xF7);
	xlsb = read(0xF8);

	up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-_OSS);

	return up;
}

//Read 1 byte from address
char BMP085::read(unsigned char address){
	unsigned char data;

	Wire.beginTransmission(BMP085_ADDRESS);
	Wire.write(address);
	Wire.endTransmission();

	Wire.requestFrom(BMP085_ADDRESS, 1);
	while(!Wire.available());

	return Wire.read();
}

//Read 2 bytes (1 int) from address and address + 1
int BMP085::readInt(unsigned char address){
	unsigned char msb, lsb;

	Wire.beginTransmission(BMP085_ADDRESS);
	Wire.write(address);
	Wire.endTransmission();

	Wire.requestFrom(BMP085_ADDRESS, 2);
	while(!Wire.available());
	msb = Wire.read();
	lsb = Wire.read();

	return (int) msb<<8 | lsb;
}
