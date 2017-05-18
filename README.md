# mbed-ttnmapper
Mbed implementation of the ttnmapper using RFM95 and Adafruit GPS

## Installation

Clone the project using git.
```
git clone https://github.com/sillevl/mbed-ttnmapper.git
```

Go into the cloned directory and start the `mbed deploy` command to download the libraries.

```
cd .\mbed-ttnmapper\
mbed deploy
```

In the src directory, you need to create a `settings.h` file. In this file
the LoRaWAN ABP settings need to be configured. You can copy
the example file, and edit it to your preferences.

```
cp .\src\settings.example.h .\src\settings.h
```

Now you can build the program and flash it to your mbed
```
mbed compile -f -m LPC1768 -t GCC_ARM
```
