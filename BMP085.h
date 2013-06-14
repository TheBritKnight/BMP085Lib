/////////////////////////////////////////////////////////////////
//                      BMP085 Library                         //
//           Written by Charles Dawson, June 2013              //
//              Released under the MIT License                 //
/////////////////////////////////////////////////////////////////

//Special thanks to the folks at bildr.org and sparkfun.com (particularly Jim Lindblom).
	//Most of this code is from their tutorial, 
	 //just wrapped up nicely in library form

/*
Copyright (c) 2013 Charles Dawson

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

#ifndef BMP085_h
#define BMP085_h

#include "Arduino.h"
#include "Wire.h"

class BMP085{
	public:
		BMP085(unsigned char OSS);
		void calibrate();
		float getTemperature();
		float getPressure();
	private:
		char read(unsigned char address);
		int readInt(unsigned char address);

		unsigned int getRawTemp();
		unsigned long getRawPressure();

		//Oversampling setting
		unsigned char _OSS;

		//Calibration values
		int _ac1;
		int _ac2;
		int _ac3;
		unsigned int _ac4;
		unsigned int _ac5;
		unsigned int _ac6;
		int _b1;
		int _b2;
		int _mb;
		int _mc;
		int _md;
		long _b5;
};
#endif
