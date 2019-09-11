#!/usr/bin/env python3
from Adafruit_BBIO.SPI import SPI
from time import sleep

# /dev/spidev1.0

def Watering():
    print("watering")

    spi = SPI(1, 0)
    spi.msh = 100000
    spi.bpw = 8

    list = []
    i=0
    while i<50:
        spi.writebytes([0x31])
        sleep(0.1)
        list.append(spi.readbytes(3))
        print(list.pop())
        ++i
        sleep(0.5)
    spi.close()


def PullData():
    print("pull data")

    spi = SPI(1, 0)
    spi.msh = 100000
    spi.bpw = 8

    spi.writebytes([0x30])
    sleep(0.1)
    moistureVals = spi.readbytes(3)
    moisture = str(moistureVals[0]) + str(moistureVals[1]) + str(moistureVals[2])

    spi.writebytes([0x31])
    sleep(0.1)
    moistureVals1 = spi.readbytes(3)
    moisture1 = str(moistureVals1[0]) + str(moistureVals1[1]) + str(moistureVals1[2])

    spi.writebytes([0x35])
    sleep(0.1)
    lightVals = spi.readbytes(3)
    light = str(lightVals[0]) + str(lightVals[1]) + str(lightVals[2])

    Data = []
    Data.append(moisture)
    Data.append(moisture1)
    Data.append(light)

    spi.close()
    return Data


def getMoist_ch0():
    #print("Reading Moisture CH 0")

    spi = SPI(1, 0)
    spi.msh = 100000
    spi.bpw = 8

    spi.writebytes([0x30])
    sleep(0.1)
    moistureVals = spi.readbytes(3)
    moisture = str(moistureVals[0]) + str(moistureVals[1]) + str(moistureVals[2])

    spi.close()
    return moisture

def getMoist_ch1():
    #print("Reading Moisture CH 1")

    spi = SPI(1, 0)
    spi.msh = 100000
    spi.bpw = 8

    spi.writebytes([0x31])
    sleep(0.01)
    moistureVals = spi.readbytes(3)
    moisture = str(moistureVals[0]) + str(moistureVals[1]) + str(moistureVals[2])

    spi.close()
    return moisture

def getMoist_ch1():
    #print("Reading Moisture CH 1")

    spi = SPI(1, 0)
    spi.msh = 100000
    spi.bpw = 8

    spi.writebytes([0x31])
    sleep(0.01)
    moistureVals = spi.readbytes(3)
    moisture = str(moistureVals[0]) + str(moistureVals[1]) + str(moistureVals[2])

    spi.close()
    return moisture

def getMoist_ch2():
    #print("Reading Moisture CH 2")

    spi = SPI(1, 0)
    spi.msh = 100000
    spi.bpw = 8

    spi.writebytes([0x32])
    sleep(0.01)
    moistureVals = spi.readbytes(3)
    moisture = str(moistureVals[0]) + str(moistureVals[1]) + str(moistureVals[2])

    spi.close()
    return moisture

def getLight():
    #print("Reading Light Data")

    spi = SPI(1, 0)
    spi.msh = 100000
    spi.bpw = 8

    spi.writebytes([0x35])
    sleep(0.01)
    lightVals = spi.readbytes(3)
    light = str(lightVals[0]) + str(lightVals[1]) + str(lightVals[2])

    spi.close()
    return light
