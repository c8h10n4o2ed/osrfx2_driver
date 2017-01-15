Readme file for osrfx2 userspace driver:

Dependencies (Ubuntu 16.04.1):
    $ sudo apt-get install libusb-1.0-0-dev

Usage:
    Enable all LEDs (except for the top two) and display switch state:
        $ ./bin/osrfx2 -l ff -s

    Disable all LEDs:
        $ ./bin/osrfx2 -l 0

Command Line Switches:
    -l <level_hex>  Sets the LEDs to this level.
    -s              Reads the switch state using EP1 and via a control transfer request.


