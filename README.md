NTP Projection Clock
====================

A NodeMCU based clock that wirelessly connects to your network and runs
an NTP client to retrieve the time and project it onto a wall or ceiling.

Setup
-----

Install the Arduino IDE 1.8 or later and NodeMCU Arduino support:

    https://www.arduino.cc/

Launch the IDE with:

    cd /usr/share/arduino; arduino

Install [NodeMCU Arduino Support](https://github.com/esp8266/Arduino#installing-with-boards-manager):

* Start Arduino and open Preferences window.
* Enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.
* Open Boards Manager from Tools->Board menu and install or update the esp8266 platform.
* Under Tools->Boards, select "NodeMCU 1.0 (ESP-12E Module)".

Load the `firmware.ino` sketch, select NodeMCU board and board, verify and upload.

To set the Wifi password dynamically, shell into the `scripts` directory and run:

    cd src/scripts
    ./init_virtualenv.sh
    . .env/bin/activate
    ./clock_controller.py login ssid ssidpwd

Otherwise, create the file src/firmware/credentials.h and enter your credentials there, recompile and upload your firmware.

Debugging
---------

To inspect output, select "Serial Monitor" under Tools in Arduino IDE. Select 115200 baudrate.

Also, via the clock_controller.py script with:

    ./clock_controller.py monitor

To get detailed time output:

    ./clock_controller.py time
