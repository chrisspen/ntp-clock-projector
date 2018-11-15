# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile
# make show_boards

BOARD_TAG    = uno
#BOARD_TAG    = mega
#BOARD_SUB    = atmega2560
#BOARD_TAG    = uno_pro
MONITOR_PORT = /dev/ttyACM0

# Nano
#BOARD_TAG    = nano
#BOARD_SUB   = atmega328
#MONITOR_PORT = /dev/ttyUSB0

AVRDUDE_OPTS = -v
#ARDUINO_LIBS = ros_lib Wire Adafruit_BNO055 MemoryFree
ARDUINO_LIBS = Adafruit_GFX Adafruit_LED_Backpack Wire SPI
#ARDUINO_SKETCHBOOK = /home/$(USER)/sketchbook
ARDUINO_SKETCHBOOK = $(realpath .)/lib
USER_LIB_PATH = $(realpath .)/lib
include /usr/share/arduino/Arduino.mk
