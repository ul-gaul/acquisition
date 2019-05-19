#A TESTER : Est-ce que le microcontroller a TOUT LE TEMPS le temps de recevoir un nouveau paquet GPS avant la transmission d'un rocketpackeet

desc = "USB Serial Port (COM3)"
hwid = "USB"
VID= "PID=0403:6001"
SER = "A603LDNUA"

toSend1 = "$GPGGA,111111.932,2447.0949,N,12100.5223,E,1,11,0.8,118.2,M,,,,0000*02\r\n"
toSend2 = "$GPGLL,2222.0944,N,12100.5213,E,222222.932,A,A*57\r\n"
toSend3 = "hiugrthiugrt riwiw i01032498 hitler did something wrong gtogrtoijgroij  rjoigrt $GPRMC,444444.932,A,4242.0949,N,42424.5223,E,000.0,000.0,030407,,,A*61\r\n hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjhhhhhhhhhhhhh"
import serial
with open("fichier.txt",'a')as heyho:
    with serial.Serial('COM3',9600) as ser:
        #ser.write(toSend3.encode())
        while True:
            p = ser.readline().decode("unicode_escape")
            print(p)
            heyho.write(p)