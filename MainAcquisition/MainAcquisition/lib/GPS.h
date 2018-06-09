/* mbed EM-406 GPS Module Library
 * Copyright (c) 2008-2010, sford
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mbed.h"
#include "stdio.h"

#ifndef MBED_GPS_H
#define MBED_GPS_H

/**  A GPS interface for reading from a Globalsat EM-406 GPS Module */
class GPS {
public:

    /** Create the GPS interface, connected to the specified serial port
     */    
    GPS(PinName tx, PinName rx);
    
    /** Sample the incoming GPS data, returning whether there is a lock
     * 
     * @return 1 if there was a lock when the sample was taken (and therefore .longitude and .latitude are valid), else 0
     */
    int sample();
    
    /** The longitude (call sample() to set) */
    float longitude;

    /** The latitude (call sample() to set) */
    float latitude;
    
    int num_sat;
    float hori_dilute;
    float alt;
    float geoid, time;
    char ns, ew;
	char gu, hu;
     
private:
    float trunc(float v);
    void getline();
    
    Serial _gps;
    char msg[256];

};

#endif
