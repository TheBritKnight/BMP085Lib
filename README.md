BMP085Lib
=========

This is a library for interfacing with the BMP085 pressure sensor.

Credit to the folks at bildr.org. Most of this code was taken from their turtorial, and I just packed it into a library for ease of use.


Notes on usage:
---------------

The board will only accept 3.3V! Trying to use more than this WILL damage your sensor. This means that you will need a level shifter if you are using an Uno or other 5V 'duino.
The CL pin on the sensor should be wired to A5, and the DA pin to A4.
You need to call Wire.begin() in your setup() method.
Temperatures are given in °C, pressures in atm. It shouldn't be too difficult to do whatever you want to convert these.

A post-script:
--------------

The original code and tutorial can be found [here](http://bildr.org/2011/06/bmp085-arduino/ "Bildr Tutorial"). I did not include the altitude method, and a number of others, because I didn't need them for my application. Buy hey, that's what open source is for. If you need the altitude function, you can implement it and make the world a better place :) . This work is released under the MIT License (see LICENSE.txt for more info).