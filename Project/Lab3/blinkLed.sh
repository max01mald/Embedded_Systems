#!/usr/bin/env bash

# tested on beaglebone black rev C

# valid argument passing is considered:
# a single argument: gpio pin
# or two arguments: gpio pin, rate of change of state (ON/OFF)

if [[ ("$#" -lt 1 || "$#" -gt 2) ]]; then
    echo "Illegal number of parameters"
    echo "Usage: ./blinkLed <gpio_pin_number>"
    echo "Usage: ./blinkLed <gpio_pin_number> <rate_of_change>"
    echo "Example: ./blinkLed 44"
    echo "Example: ./blinkLed 44 2"
    exit 1
fi

PIN_NUMBER=$1

# input validation
if [[ $1 =~ ^[0-9]+$ ]]; then
    PIN_NUMBER=$1
else
    echo "$1 is not a digit"
    exit 1
fi

USER_SLEEP_TIME=$2
SLEEP_TIME=1

# -z check returns true if following string length is 0
if [ ! -z $2 ]; then
    if [[ $2 =~ ^[0-9]+$ ]]; then
        SLEEP_TIME="${USER_SLEEP_TIME:0:1}"
    else
        echo "$2 is not a digit"
        exit 1
    fi
fi


GPIO_DIR="/sys/class/gpio"
THIS_GPIO_DIR="$GPIO_DIR/gpio$PIN_NUMBER"

# check if gpio pin already exported, if not then export it
if [ ! -d $THIS_GPIO_DIR ]; then
    echo "Exporting gpio pin $1"
    echo $PIN_NUMBER > $THIS_GPIO_DIR
fi

# setup trap on exit, to make sure to turn off led on terminating script
trap "echo low > $THIS_GPIO_DIR/direction; exit 1" INT


# ctrl-C to exit
while true
do
    # turn on LED
    echo high > $THIS_GPIO_DIR/direction
    # wait
    sleep $SLEEP_TIME
    # turn off LED
    echo low > $THIS_GPIO_DIR/direction
    # wait
    sleep $SLEEP_TIME
done
