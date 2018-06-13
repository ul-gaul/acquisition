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
    _gps.baud(115200);
      
    longitude = 0.0;
    latitude = 0.0;
    ns = 'N';
    ew = 'E';
    alt = 0.0;
            
}

int GPS::sample() {
	//float time, hori_dilute, alt,geoid;
	int sscanferr;
    int lock;
	char buf[256];
    
    getline();
	char* token;
	token = strtok(msg, ",");
	if(strcmp(token, "GPGGA") != 0) {
		return 0;
	}
	for(int i = 0; i < 8; i++) {
		token = strtok(NULL, ",");
		if(token == NULL) {
			return 0;
		}
		if(i == 0) {
			// get time
			time = atof(token);
		} else if(i == 1) {
			// get latitude
			latitude = atof(token);
			latitude /= 100;
		} else if(i == 2) {
			// get north-south char
			ns = token[0];
		} else if(i == 3) {
			// get longitude
			longitude = atof(token);
			longitude /= 100;
		} else if(i == 4) {
			// get east-west char
			ew = token[0];
		} else if(i == 5) {
			// get lock on value
			lock = atoi(token);
		} else if(i == 6) {
			// get number of satellites locked on
			num_sat = atoi(token);
		} else {
			break;
		}
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
