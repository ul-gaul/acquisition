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
 
#include "GPS.h"

//Serial pc1(USBTX, USBRX);

GPS::GPS(PinName tx, PinName rx) : _gps(tx, rx) {
    _gps.baud(9600);
      
    longitude = 0.0;
    latitude = 0.0;
    ns = 'N';
    ew = 'E';
    alt = 0.0;
            
}

int GPS::sample() {
    //float time, hori_dilute, alt,geoid;
   
    int lock;
    
    while(1) {        
        getline();
        // Check if it is a GPGGA msg (matches both locked and non-locked msg)
        //pc1.printf(msg);
        //pc1.printf("\n");
        wait(5);
        //pc1.printf("display msg\r\n");
        //pc1.printf(msg);
        //pc1.printf("\n");
        if(sscanf(msg, "GPGGA,%f,%f,%c,%f,%c,%d,%d,%f,%f,%c,%f,%c", &time, &latitude, &ns, &longitude, &ew, &lock, &num_sat, &hori_dilute, &alt, &hu, &geoid, &gu/*, &age_diff, &diff_ID*/) >= 1) { 
            if(!lock) {
                longitude = 0.0;
                latitude = 0.0;  
                ns = 'Z';
                ew = 'Z';
                alt = 0.0;      
                return 0;
            } else {
                //if(ns == 'S') {    latitude  *= -1.0; }
//                if(ew == 'W') {    longitude *= -1.0; }
//                float degrees = trunc(latitude / 100.0f);
//                float minutes = latitude - (degrees * 100.0f);
//                latitude = degrees + minutes / 60.0f;    
//                degrees = trunc(longitude / 100.0f * 0.01f);
//                minutes = longitude - (degrees * 100.0f);
//                longitude = degrees + minutes / 60.0f;
//                pc1.printf(msg);
                //pc1.printf("\n\rlongitude is %f\n\r", longitude);
                //pc1.printf("\n\rtime is %f\n\r", time);
                //pc1.printf("ns is %c\n\r", ns);
                //pc1.printf("ew is %c\n\r", ew);
                //pc1.printf("alt is %f\n\r", alt);
                
                latitude /= 100;
                longitude /= 100;
                
                return 1;
            }
        
        }
        return 0;
    }
}

float GPS::trunc(float v) {
    if(v < 0.0) {
        v*= -1.0;
        v = floor(v);
        v*=-1.0;
    } else {
        v = floor(v);
    }
    return v;
}

void GPS::getline() {
    int i;
    char a;
    int n;
    while(1) {
	//strcpy(msg, '\0');
	msg[0] = 0;
    //pc1.printf("\r\n\n New Message: ");
//    pc1.printf(msg);
//    pc1.printf("\r\n\n");    
    i = 0;
    a = _gps.getc();
    msg[i] = a;
    if (a == '$') {
        //pc1.printf("%c",a);
        a = _gps.getc();
        msg[i] = a;
        i++;
        if (a == 'G') {
            //pc1.printf("%c",a);
            a = _gps.getc();
            msg[i] = a;
            i++;
            if (a == 'P') {
                //pc1.printf("%c",a);
                a = _gps.getc();
                msg[i] = a;
                i++;
                if (a == 'G') {
                    //pc1.printf("%c",a);
                    a = _gps.getc();
                    msg[i] = a;
                    i++;
                    if (a == 'G') {
                        //pc1.printf("%c",a);
                        a = _gps.getc();
                        msg[i] = a;
                        i++;
                        if (a == 'A') {
                            //pc1.printf("%c",a);
                            //a = _gps.getc();
                            //msg[i] = a;
                            //pc1.printf(msg);
                            //pc1.printf("\r\n");
        
                            for (n = 5; n < 456; n++) {
                               msg[n] = _gps.getc();
                               //pc1.printf("%c", msg[n]);
                               if(msg[n] == '\r') {
                                 msg[n] = '0';
                                 return;
                                }
                            }
                         }
                     }
                 }
            }
       }
       }                          
//    while(_gps.getc() != '$') {
//        //char a = _gps.getc();
//        for(i = 0; i < 256; i++) {
//            msg[i] = _gps.getc();
//            pc1.printf("%c", msg[i]);
//            if(msg[i] == '\r') {
//             msg[i] = 0;
//             return;
//                }
//                }
//          
//         
//    }
//    while(_gps.getc() != '$');    // wait for the start of a line
//    for(int i=0; i<256; i++) {
//        msg[i] = _gps.getc();
//        if(msg[i] == '\r') {
//            msg[i] = 0;
//            return;
//        }
//    }
//    error("Overflowed message limit");
}
}
