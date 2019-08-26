import serial
import time
from serial.tools import list_ports

Ports = list(list_ports.comports())

for Port in Ports:
    print(Port)

Port = input("Enter the serial port:(COM1)\n")
Baudrate = 115200

Ser = serial.Serial(Port, Baudrate)
print(Ser.get_settings())


def SerialSave(n):
    with open("SerialData.txt", 'a', newline='') as File:
        for i in range(n):
            LocalTime = time.ctime(time.time())
            SerialData = Ser.read_until()
            print(SerialData)
            StrSerial = str(SerialData.decode('utf-8'))
            StrSerial.replace('\r\n', '')
            File.writelines(LocalTime + ' ' + StrSerial)


SerialSave(1000)
Ser.close()
