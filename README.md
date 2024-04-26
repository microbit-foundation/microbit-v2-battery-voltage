# BBC micro:bit V2 Battery Voltage Project

A C++ example project showing how to measure the BBC micro:bit V2 battery
voltage.

All the ADC channels are currently used for the Analogue pins and the on-board
microphone, so for this project we hijack the ADC channel used for P0 and
re-route it to the internal Vdd (voltage from the microcontroller power input).

## Building the project

### Dependencies

- [GNU Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
- [Git](https://git-scm.com)
- [CMake](https://cmake.org/download/)
- [Python 3](https://www.python.org/downloads/)

For example, on Ubuntu:

```
sudo apt install gcc
sudo apt install git
sudo apt install cmake
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi
```

### Building

- Install the dependencies
- Clone this repository
  ```
  git clone https://github.com/microbit-foundation/microbit-v2-battery-voltage.git
  ```
- In the root of this repository run the `build.py` script
  ```
  cd microbit-v2-battery-voltage
  python build.py
  ```
- The `MICROBIT.hex` hex file will be built and placed in the root folder.
- Copy the `MICROBIT.hex` file into the `MICORBIT` USB drive to flash the
  micro:bit
