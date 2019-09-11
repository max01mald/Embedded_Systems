#!/usr/bin/env bash
config-pin P9.17 spi_cs
config-pin P9.18 spi
config-pin P9.21 spi
config-pin P9.22 spi_sclk

# this runs the sensor data logger in the background
python3 /home/debian/SOEN422-Project/smrtpot/BBB_Arduino_Interface/API.py
