#!/usr/bin/env python3
from time import sleep
from Comm import PullData, getMoist_ch0, getMoist_ch1, getMoist_ch2, getLight
from Data import CreateDir, writeSeconds

def PackagePull():
    Data = PullData()
    print("Moisture CH 0 " + Data[0])
    print("Moisture CH 1 " + Data[1])
    print("Light " + Data[2])

def SmartPull():

    M0 = getMoist_ch0()
    M1 = getMoist_ch1()
    M2 = getMoist_ch2()
    L = getLight()

    if(int(M0) > 10):
        print("Moisture CH 0 " + M0)

    if(int(M1) > 10):
        print("Moisture CH 1 " + M1)

    if(int(M2) > 10):
        print("Moisture CH 2 " + M2)

    if(int(L) > 10):
        print("Light " + L)

def Update_Stats(Garden):

    CreateDir(Garden)
    M0 = getMoist_ch0()
    M1 = getMoist_ch1()
    M2 = getMoist_ch2()
    L = getLight()
    print("light: " + str(L))

    if(int(M0) > 10):
        writeSeconds(Garden, "Moist_CH0", M0)

    if(int(M1) > 10):
        writeSeconds(Garden, "Moist_CH1", M1)

    if(int(M2) > 10):
        writeSeconds(Garden, "Moist_CH2", M2)

    if(int(L) > 10):
        writeSeconds(Garden, "Light", L)

def Recording():
    print("Recording")
    while 1:
        Update_Stats("Garden")
        sleep(1)

Recording()
