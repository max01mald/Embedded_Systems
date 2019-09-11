#!/usr/bin/env bash
WEBAPP_DIR="/home/debian/SOEN422-Project/smrtpot"

cd $WEBAPP_DIR

#. venv/bin/activate # removed because it doesn't allow us to use the system Adafruit_BBIO libary
# had to install the flask/flask-bootstrap dependencies on the system
./app.py
