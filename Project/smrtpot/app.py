#!/usr/bin/env python3
# This is the controller page of the Flask GUI
# Flask is light-weight and modular so this is actually all we need to set up a simple HTML page
import flask
import sqlite3
from flask import Flask, request, url_for, session
import sqlite3 as sql
import os.path
import time
import random
import subprocess
from Adafruit_BBIO.SPI import SPI

app = flask.Flask(__name__)
# necessary for session variables to work
app.secret_key = "any random string"

DB = "./database.db"

@app.route("/")
def index():
    con = sql.connect(DB)
    con.row_factory = sql.Row

    cur = con.cursor()
    cur.execute("SELECT plants.plantName, plants.isCactus, plants.startTime, wateringSchedules.scheduleName " +
                "FROM [plants] JOIN wateringSchedules " +
                "ON [plants].wateringSchedule = wateringSchedules.id"
    )

    plants = cur.fetchall()
    # debugging
#    print("len(rows): " + str(len(rows)))

    # more debugging
#    for row in rows:
#        print("plantName: " + str(plants['plantName']))
#        print("isCactus:" + str(plants['isCactus']))
#        print("startTime: " + str(plants['startTime']))
#        print("scheduleName: " + str(plants['scheduleName']))

    # default
    if session.get("selected_schedule") != None:
        print("selected_schedule: " + str(session.get("selected_schedule")))
        selectedSchedule = session.get("selected_schedule")
    else:
        print("selected_schedule is empty")
        selectedSchedule = "Non-Cactus default"

    if session.get("watering_command_status") != None:
        print("watering_command_status: " + str(session.get("watering_command_status")))
        waterCommandStatus = session.get("watering_command_status")
    else:
        print("watering_command_status is empty")
        waterCommandStatus = "OFF"

    # group all daily blocks for particular watering schedule and sort by day of week, start time
    cur.execute("SELECT startTime, durationSeconds, dayOfWeek FROM dailyBlocks WHERE wateringSchedule = '" +
                selectedSchedule + "' ORDER BY dailyBlocks.dayOfWeek, dailyBlocks.startTime ASC"
    )

    dailyBlocks = cur.fetchall()

    # debugging
    print("len(dailyBlocks): " + str(len(dailyBlocks)))

    # more debugging
    # for block in dailyBlocks:
        # print("dayOfWeek: " + str(block['dayOfWeek']))
        # print("durationSeconds:" + str(block['durationSeconds']))
        # print("startTime: " + str(block['startTime']))

    cur.execute("SELECT * FROM wateringSchedules")
    schedules = cur.fetchall()

    return flask.render_template("Garden.html", plantRows = plants, blockRows = dailyBlocks, scheduleRows = schedules, selectedSchedule = selectedSchedule, waterCommandStatus = waterCommandStatus)

@app.route("/startWatering", methods = ["POST"])
def startWatering():
    print("Started watering...")

    session['watering_command_status'] = "ON"

    # send the signal to arduino to start watering
    spi = SPI(1, 0)
    spi.msh = 100000
    spi.bpw = 8

    # send the appropriate signal
    spi.writebytes([0x36]) # '6'

    print(spi.readbytes(1))

    spi.close()

    return flask.redirect("/")

@app.route("/stopWatering", methods = ["POST"])
def stopWatering():
    print("Stopping watering")

    session['watering_command_status'] = "OFF"

    # send the signal to arduino to stop watering
    spi = SPI(1, 0)
    spi.msh = 100000
    spi.bpw = 8

    # send the appropriate signal
    spi.writebytes([0x37]) # '7'

    print(spi.readbytes(1))

    spi.close()

    return flask.redirect("/")

@app.route("/selectSchedule", methods = ["POST"])
def selectSchedule():
    print("selectSchedule")
    if request.method == "POST":
        selectedSchedule = request.form['selectedSchedule']

        # save into global session variable
        session['selected_schedule'] = selectedSchedule
        # redirect to rerender the schedule
        # not optimal but works

        return flask.redirect(url_for("index"))


@app.route("/addPlant", methods = ["POST"])
def addPlant():
    print("adding plant to DB")
    if request.method == "POST":
        try:
            plantName = request.form['plantName']
            print("plantName: " + str(plantName))
            plantType = request.form['plantType']
            print("plantType: " + str(plantType))

            startTime = time.strftime('%Y-%m-%d %H:%M:%S')

            isCactus = 1 if plantType == "Cactus" else 0

            print(str(isCactus))

            with sql.connect(DB) as con:
                cur = con.cursor()
                cur.execute("INSERT INTO plants (plantName,startTime,isCactus,wateringSchedule) VALUES (?,?,?,?)", (plantName,startTime,isCactus,(isCactus + 1)))
                con.commit()
                msg = "Record successfully added"
        except:
            con.rollback()
            msg = "error in insert operation"

        finally:
            print(msg)
            con.close()

            # redirect ensures refreshing page won't submit form again
            return flask.redirect("/")

@app.route("/removePlant", methods = ["POST"])
def removePlant():
    print("removing plant from DB")
    if request.method == "POST":
        try:
            plantName = request.form["plantName"]

            with sql.connect(DB) as con:
                cur = con.cursor()
                cur.execute("DELETE FROM plants WHERE plantName='" + plantName + "'")
                con.commit()
                msg = "Record successfully deleted"

        except:
            con.rollback()
            msg = "error in delete operation"

        finally:
            print(msg)
            con.close()

            # redirect ensures refreshing page won't submit form again
            return flask.redirect("/")

def subprocess_cmd(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    proc_stdout, error = process.communicate()
    return proc_stdout.decode()

def getTail(sensor, column):
    return "tail -1 BBB_Arduino_Interface/Statistics/Garden/" + sensor + "/" + column + ".txt"

@app.route("/Stats", methods = ["POST", "GET"])
def Stats():
    print("/Stats hit")

    #@TODO: learn how to pass dictionaries as template variables (attemps where made in vain)
    # Light sensor
    cmd = getTail("Light", "Seconds")
    print("cmd: " + str(cmd))
    lightSeconds = subprocess_cmd(cmd)

    cmd = getTail("Light", "Minutes")
    print("cmd: " + str(cmd))
    lightMinutes = subprocess_cmd(cmd)

    cmd = getTail("Light", "Hours")
    print("cmd: " + str(cmd))
    lightHours = subprocess_cmd(cmd)

    cmd = getTail("Light", "Days")
    print("cmd: " + str(cmd))
    lightDays = subprocess_cmd(cmd)

    cmd = getTail("Light", "Weeks")
    print("cmd: " + str(cmd))
    lightWeeks = subprocess_cmd(cmd)

    cmd = getTail("Light", "Months")
    print("cmd: " + str(cmd))
    lightMonths = subprocess_cmd(cmd)

    cmd = getTail("Light", "Years")
    print("cmd: " + str(cmd))
    lightYears = subprocess_cmd(cmd)

    # moisture 0
    cmd = getTail("Moist_CH0", "Seconds")
    print("cmd: " + str(cmd))
    moist0Seconds = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH0", "Minutes")
    print("cmd: " + str(cmd))
    moist0Minutes = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH0", "Hours")
    print("cmd: " + str(cmd))
    moist0Hours = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH0", "Days")
    print("cmd: " + str(cmd))
    moist0Days = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH0", "Weeks")
    print("cmd: " + str(cmd))
    moist0Weeks = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH0", "Months")
    print("cmd: " + str(cmd))
    moist0Months = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH0", "Years")
    print("cmd: " + str(cmd))
    moist0Years = subprocess_cmd(cmd)

    # moisture 1
    cmd = getTail("Moist_CH1", "Seconds")
    print("cmd: " + str(cmd))
    moist1Seconds = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH1", "Minutes")
    print("cmd: " + str(cmd))
    moist1Minutes = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH1", "Hours")
    print("cmd: " + str(cmd))
    moist1Hours = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH1", "Days")
    print("cmd: " + str(cmd))
    moist1Days = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH1", "Weeks")
    print("cmd: " + str(cmd))
    moist1Weeks = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH1", "Months")
    print("cmd: " + str(cmd))
    moist1Months = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH1", "Years")
    print("cmd: " + str(cmd))
    moist1Years = subprocess_cmd(cmd)

    # moisture 2
    cmd = getTail("Moist_CH2", "Seconds")
    print("cmd: " + str(cmd))
    moist2Seconds = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH2", "Minutes")
    print("cmd: " + str(cmd))
    moist2Minutes = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH2", "Hours")
    print("cmd: " + str(cmd))
    moist2Hours = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH2", "Days")
    print("cmd: " + str(cmd))
    moist2Days = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH2", "Weeks")
    print("cmd: " + str(cmd))
    moist2Weeks = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH2", "Months")
    print("cmd: " + str(cmd))
    moist2Months = subprocess_cmd(cmd)

    cmd = getTail("Moist_CH2", "Years")
    print("cmd: " + str(cmd))
    moist2Years = subprocess_cmd(cmd)

    return flask.render_template("Stats.html", lightSeconds = lightSeconds, lightMinutes = lightMinutes, lightHours = lightHours,
                                 lightDays = lightDays, lightWeeks = lightWeeks, lightMonths = lightMonths, lightYears = lightYears,
                                 moist0Seconds = moist0Seconds, moist0Minutes = moist0Minutes, moist0Hours = moist0Hours,
                                 moist0Days = moist0Days, moist0Weeks = moist0Weeks, moist0Months = moist0Months, moist0Years = moist0Years,
                                 moist1Seconds = moist1Seconds, moist1Minutes = moist1Minutes, moist1Hours = moist1Hours,
                                 moist1Days = moist1Days, moist1Weeks = moist1Weeks, moist1Months = moist1Months, moist1Years = moist1Years,
                                 moist2Seconds = moist2Seconds, moist2Minutes = moist2Minutes, moist2Hours = moist2Hours,
                                 moist2Days = moist2Days, moist2Weeks = moist2Weeks, moist2Months = moist2Months, moist2Years = moist2Years
    )

app.run(
    debug=True,
    host='0.0.0.0' # hosting on machine IP and usb 1 ip defaulted to 192.168.6.2
)
