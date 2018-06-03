/** LSM303DLHC Interface Library
 *
 * base on code by Michael Shimniok http://bot-thoughts.com
 *
 *  and on test program by @tosihisa and 
 *
 *  and on Pololu sample library for LSM303DLHC breakout by ryantm:
 *
 * Copyright (c) 2011 Pololu Corporation. For more information, see
 *
 * http://www.pololu.com/
 * http://forum.pololu.com/
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#include "mbed.h"
#include "LSM303DLHC.h"


const int addr_acc = 0x32;
const int addr_mag = 0x3c;

enum REG_ADDRS {
    /* --- Mag --- */
    CRA_REG_M   = 0x00,
    CRB_REG_M   = 0x01,
    MR_REG_M    = 0x02,
    OUT_X_M     = 0x03,
    OUT_Y_M     = 0x05,
    OUT_Z_M     = 0x07,
    /* --- Acc --- */
    CTRL_REG1_A = 0x20,
    CTRL_REG4_A = 0x23,
    OUT_X_A     = 0x28,
    OUT_Y_A     = 0x2A,
    OUT_Z_A     = 0x2C,
};

bool LSM303DLHC::write_reg(int addr_i2c,int addr_reg, char v)
{
    char data[2] = {addr_reg, v}; 
    return LSM303DLHC::_LSM303.write(addr_i2c, data, 2) == 0;
}

bool LSM303DLHC::read_reg(int addr_i2c,int addr_reg, char *v)
{
    char data = addr_reg; 
    bool result = false;
    
    __disable_irq();
    if ((_LSM303.write(addr_i2c, &data, 1) == 0) && (_LSM303.read(addr_i2c, &data, 1) == 0)){
        *v = data;
        result = true;
    }
    __enable_irq();
    return result;
}


LSM303DLHC::LSM303DLHC(PinName sda, PinName scl):
    _LSM303(sda, scl)
{
    char reg_v;
    _LSM303.frequency(200000);
        
    reg_v = 0;
    
    reg_v |= 0x37;          /* X/Y/Z axis enable. */
    write_reg(addr_acc,CTRL_REG1_A,reg_v);

    reg_v = 0;
   // reg_v |= 0x01 << 6;     /* 1: data MSB @ lower address */
    reg_v = 0x01 << 4;     /* +/- 4g */
    write_reg(addr_acc,CTRL_REG4_A,reg_v);

    /* -- mag --- */
    reg_v = 0;
    reg_v |= 0x06 << 2;     /* Minimum data output rate = 15Hz */
    write_reg(addr_mag,CRA_REG_M,reg_v);

    reg_v = 0;
    reg_v |= 0x01 << 5;     /* +-1.3Gauss */
    //reg_v |= 0x07 << 5;     /* +-8.1Gauss */
    write_reg(addr_mag,CRB_REG_M,reg_v);

    reg_v = 0;              /* Continuous-conversion mode */
    write_reg(addr_mag,MR_REG_M,reg_v);
}


bool LSM303DLHC::read(float *ax, float *ay, float *az, float *mx, float *my, float *mz) {
    char acc[6], mag[6];
 
    if (recv(addr_acc, OUT_X_A, acc, 6) && recv(addr_mag, OUT_X_M, mag, 6)) {
        *ax = float(short(acc[1] << 8 | acc[0]))/8192;  //32768/4=8192
        *ay =  float(short(acc[3] << 8 | acc[2]))/8192;
        *az =  float(short(acc[5] << 8 | acc[4]))/8192;
        //full scale magnetic readings are from -2048 to 2047
        //gain is x,y =1100; z = 980 LSB/gauss
        *mx = float(short(mag[0] << 8 | mag[1]))/1100;
        *mz = float(short(mag[2] << 8 | mag[3]))/980;
        *my = float(short(mag[4] << 8 | mag[5]))/1100;
 
        return true;
    }
 
    return false;
}


bool LSM303DLHC::recv(char sad, char sub, char *buf, int length) {
    if (length > 1) sub |= 0x80;
 
    return _LSM303.write(sad, &sub, 1, true) == 0 && _LSM303.read(sad, buf, length) == 0;
}
