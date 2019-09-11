#/usr/bin/env python3
import os
import datetime

def CreateDir(Name):
    current_dir = os.path.dirname(os.path.realpath(__file__))

    current_dir = current_dir + "/Statistics/"

    # cd into Statistics
    os.chdir(current_dir)

    if not os.path.exists(Name):
        os.makedirs(Name)
        CreateFiles(Name)

def CreateFiles(Name):

    current_dir = os.path.dirname(os.path.realpath(__file__))

    current_dir = current_dir + "/Statistics/"

    current_dir = current_dir + "/" + Name + "/"

    os.chdir(current_dir)

    if not os.path.exists("Moist_CH0"):
        os.makedirs("Moist_CH0")

    os.chdir(current_dir + "/Moist_CH0/")

    f = open("Seconds.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Minutes.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Hours.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Days.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Weeks.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Months.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Years.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Day_Log.txt","w+")
    f.close()

    f = open("Month_Log.txt","w+")
    f.close()

    f = open("Year_Log.txt","w+")
    f.close()

    os.chdir(current_dir)

    if not os.path.exists("Moist_CH1"):
        os.makedirs("Moist_CH1")

    os.chdir(current_dir + "/Moist_CH1/")

    f = open("Seconds.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Minutes.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Hours.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Days.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Weeks.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Months.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Years.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Day_Log.txt","w+")
    f.close()

    f = open("Month_Log.txt","w+")
    f.close()

    f = open("Year_Log.txt","w+")
    f.close()

    os.chdir(current_dir)

    if not os.path.exists("Moist_CH2"):
        os.makedirs("Moist_CH2")

    os.chdir(current_dir + "/Moist_CH2/")

    f = open("Seconds.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Minutes.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Hours.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Days.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Weeks.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Months.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Years.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Day_Log.txt","w+")
    f.close()

    f = open("Month_Log.txt","w+")
    f.close()

    f = open("Year_Log.txt","w+")
    f.close()

    os.chdir(current_dir)

    if not os.path.exists("Light"):
        os.makedirs("Light")

    os.chdir(current_dir + "/Light/")

    f = open("Seconds.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Minutes.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Hours.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Days.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Weeks.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Months.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Years.txt","w+")
    f.write("0\n")
    f.close()

    f = open("Day_Log.txt","w+")
    f.close()

    f = open("Month_Log.txt","w+")
    f.close()

    f = open("Year_Log.txt","w+")
    f.close()

    os.chdir(current_dir)


def file_length(fname):
    with open(fname) as f:
        try:
            for i, l in enumerate(f):
                pass
        except ValueError:
            i = 0
    return i+1


def writeSeconds(Garden,Type,Data):

    current_dir = os.path.dirname(os.path.realpath(__file__))

    current_dir = current_dir + "/Statistics/" + Garden + "/" + Type + "/"

    os.chdir(current_dir)

    f = open("Seconds.txt","a")
    f.write(Data + "\n")
    f.close()

    if file_length("Seconds.txt") > 60:
        toMinute()
        f = open("Seconds.txt","w")
        f.write("0\n")
        f.close()

def toMinute():

    list = []
    f = open("Seconds.txt","r")

    for x in f:
        list.append(int(x))

    f.close()

    avg = sum(list)/len(list)

    Log_Min(str(avg))

    f = open("Minutes.txt","a")
    f.write(str(avg) + "\n")
    f.close()

    if file_length("Minutes.txt") > 60:
        toHour()
        f = open("Minutes.txt","w")
        f.write("0\n")
        f.close()

def Log_Min(Value):
    now = datetime.datetime.now()

    #f = open("Day_Log.txt","a")
    print(str(now.year) + "/" + str(now.month) + "/" + str(now.day) + ": " + Value + "\n")
    #f.close()

def toHour():

    list = []
    f = open("Minutes.txt","r")

    for x in f:
        list.append(int(x))

    f.close()

    avg = sum(list)/len(list)

    f = open("Hours.txt","a")
    f.write(str(avg) + "\n")
    f.close()

    if file_length("Hours.txt") > 24:
        toDay()
        f = open("Hours.txt","w")
        f.write("0\n")
        f.close()

def toDay():

    list = []
    f = open("Hours.txt","r")

    for x in f:
        list.append(int(x))

    f.close()

    avg = sum(list)/len(list)

    Log_D(str(avg))

    f = open("Days.txt","a")
    f.write(str(avg) + "\n")
    f.close()

    if file_length("Days.txt") > 30:
        toMonth()
        f = open("Days.txt","w")
        f.write("0\n")
        f.close()

def toWeek():

    list = []
    f = open("Weeks.txt","r")

    for x in f:
        list.append(int(x))

    f.close()

    avg = sum(list)/len(list)

    Log_W(str(avg))

    f = open("Weeks.txt","a")
    f.write(str(avg) + "\n")
    f.close()

    if file_length("Weeks.txt") > 4:
        toMonth()
        f = open("Weeks.txt","w")
        f.write("0\n")
        f.close()

def Log_D(Value):
    now = datetime.datetime.now()

    f = open("Day_Log.txt","a")
    f.write(str(now.year) + "/" + str(now.month) + "/" + str(now.day) + ": " + Value + "\n")
    f.close()

def Log_W(Value):
    now = datetime.datetime.now()

    f = open("Week_Log.txt","a")
    f.write(str(now.year) + "/" + str(now.month) + "/" + str(now.day) + ": " + Value + "\n")
    f.close()

def toMonth():

    list = []
    f = open("Days.txt","r")

    for x in f:
        list.append(int(x))

    f.close()

    avg = sum(list)/len(list)

    Log_M(str(avg))

    f = open("Months.txt","a")
    f.write(str(avg) + "\n")
    f.close()

    if file_length("Months.txt") > 12:
        toYear()
        f = open("Months.txt","w")
        f.write("0\n")
        f.close()

def Log_M(Value):
    now = datetime.datetime.now()

    f = open("Month_Log.txt","a")
    f.write(str(now.year) + "/" + str(now.month) + ": " + Value + "\n")
    f.close()

def toYear():

    list = []
    f = open("Months.txt","r")

    for x in f:
        list.append(int(x))

    f.close()

    avg = sum(list)/len(list)

    f = open("Years.txt","a")
    f.write(str(avg) + "\n")
    f.close()
    Log_Y(str(avg))

def Log_Y(Value):
    now = datetime.datetime.now()

    f = open("Year_Log.txt","a")
    f.write(str(now.year) + ": " + Value + "\n")
    f.close()

