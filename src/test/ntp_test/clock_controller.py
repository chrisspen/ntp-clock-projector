#!/usr/bin/env python
"""
Interface to set SSID and SSIDPWD on the NTP Projection Clock.

Usage:

    ./clock_controller.py login ssid ssidpwd
    
        Sets network login. Values are saved to EEPROM.

    ./clock_controller.py login
    
        Gets network login.
    
    ./clock_controller.py time

        Retrieves the current time, for debugging.

"""
from __future__ import print_function
import sys
import os
import time

import serial

COMMAND_SET_LOGIN = 'l'
COMMAND_GET_LOGIN = 'k'
COMMAND_GET_TIME = 't'
COMMAND_END = '\n'
COMMAND_SEP = ':'

DEFAULT_BAUDRATE = 115200

def get_serial_path():
    fn = '/dev/ttyUSB0' # Default NodeMCU ESP-12E serial interface.
    assert os.path.exists(fn), 'Device not found at %s. Are you sure it is connected?' % fn
    return fn

def get_serial_instance():
    return serial.Serial(get_serial_path(), DEFAULT_BAUDRATE)

def stream_serial_input(ser, lines=0):
    lines_received = 0
    while 1:
        if ser.in_waiting:
            print('IN:%s' % ser.readline().decode().strip())
            lines_received += 1
            if lines and lines_received >= lines:
                return
        else:
            time.sleep(0.1)

def set_login(ssid, ssidpwd):
    ser = get_serial_instance()
    ssid = ssid.strip()
    ssidpwd = ssidpwd.strip()
    assert ssid, 'No SSID given.'
    assert ssidpwd, 'No SSIDPWD given.'
    assert len(ssid) <= 255, 'SSID must be less than or equal to 255 characters.'
    assert len(ssidpwd) <= 255, 'SSIDPWD must be less than or equal to 255 characters.'
    print('OT:SSID=%s:SSIDPWD=%s' % (ssid, ssidpwd))
    cmd = '%s%s:%s\n' % (COMMAND_SET_LOGIN, ssid, ssidpwd)
    ser.write(cmd.encode())
    stream_serial_input(ser)

def get_login():
    ser = get_serial_instance()
    cmd = '%s\n' % (COMMAND_GET_LOGIN,)
    ser.write(cmd.encode())
    stream_serial_input(ser)

def get_time():
    ser = get_serial_instance()
    cmd = '%s\n' % (COMMAND_GET_TIME,)
    ser.write(cmd.encode())
    stream_serial_input(ser)

if __name__ == '__main__':
    if len(sys.argv) == 4 and sys.argv[1] == 'login':
        set_login(*sys.argv[2:])
    elif len(sys.argv) == 2 and sys.argv[1] == 'login':
        get_login()
    elif len(sys.argv) == 2 and sys.argv[1] == 'time':
        get_time()
