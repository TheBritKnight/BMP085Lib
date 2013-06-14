/////////////////////////////////////////////////////////////////
//                 Altoids Weather Station                     //
//           Written by Charles Dawson, June 2013              //
//              Released under the MIT License                 //
/////////////////////////////////////////////////////////////////

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

#include <Wire.h>;
#include "BMP085.h";

BMP085 bmp(0);

void setup(){
  Serial.begin(9600);
  Serial.println("Serial Initialized");
  
  //Initialize I2C
  Wire.begin();
  Serial.println("I2C initialized");
  
  //Calibrate sensor
  bmp.calibrate();
  Serial.println("Sensor Calibrated");
}

void loop(){
  float pressure = bmp.getPressure();
  float temp = bmp.getTemperature();
  
  Serial.print("Pressure: ");
  Serial.println(pressure, 4);
  
  Serial.print("Temp: ");
  Serial.println(temp, 2);
  
  delay(2000);
}
