#!/usr/bin/env python
import Adafruit_BBIO.GPIO as GPIO
import sys
import re
from time import sleep

num_args = len(sys.argv)

if num_args < 2 or num_args > 3:
    print "Invalid parameter count"
    print "Usage: ./blinkLed.py <physical_gpio_pin>"
    print "Usage: ./blinkLed.py <physical_gpio_pin> <state_change_rate>"
    print "Example: ./blinkLed.py P8_12"
    print "Example: ./blinkLed.py P8_12 2"

    sys.exit()

# no default for this value, but need to initialize
pin = "init"

if re.search("P[8|9]_\d{2}", str(sys.argv[1])):
    pin = str(sys.argv[1])
else:
    print "Invalid physical pin format, example: 'P8_12' or 'P9_12'"
    sys.exit()

# default sleep_time
sleep_time = 1

if num_args == 3 and re.search("\d{1}", sys.argv[2]) and int(sys.argv[2]) > 0:
    sleep_time = int(re.search("\d{1}", sys.argv[2]).group(0))
elif num_args == 3:
    print("State change rate argument must be a positive integer. Only the first digit will be used.")
    print("I.e. 10 becomes 1")
    sys.exit()

GPIO.setup(pin, GPIO.OUT)

while True:
    try:
        GPIO.output(pin, GPIO.HIGH)
        sleep(sleep_time)
        GPIO.output(pin, GPIO.LOW)
        sleep(sleep_time)
    except KeyboardInterrupt:
        # make sure to turn LED off
        GPIO.output(pin, GPIO.LOW)
        sys.exit()
