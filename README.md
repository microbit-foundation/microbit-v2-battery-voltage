# BBC micro:bit V2 Battery Voltage Measurement

A C++ example project showing how to measure the BBC micro:bit V2 battery
voltage.

This project measures the input voltage, in millivolts, from the nRF52833
microcontroller and prints it on the display and serial.

A prebuilt hex file can be download from the
[GitHub Releases page](https://github.com/microbit-foundation/microbit-v2-battery-voltage/releases).

A similar example for micro:bit V1 can be found in:
https://os.mbed.com/teams/microbit/code/microbit-battery-test/

## How is the battery voltage measured

To measure the micro:bit battery voltage, the nRF52833 (the target MCU)
microcontroller ADC is used to measure its Vdd input voltage.

All the ADC channels are currently used for the Analogue pins and the on-board
microphone, so for this example we hijack the ADC channel used for P0 and
re-route it to the internal Vdd (voltage from the microcontroller power input).

It's important to note that there are two protection diodes and a voltage
regulator between the battery connector and the microcontroller Vdd pin,
as can be seen in the
[micro:bit open source schematics](https://github.com/microbit-foundation/microbit-v2-hardware).

So, there is a voltage drop on these components that will vary on different
factors, like the current load and component temperature.
Through some of the micro:bit testing we measured this voltage
drop ranging from 210 to 300 mV (measured without any additional
load on the micro:bit power supply).
However, these values should not be considered definitive,
as they may further fluctuate under different conditions.

## Building the project

### Dependencies

- [GNU Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
- [Git](https://git-scm.com)
- [CMake](https://cmake.org/download/)
- [Python 3](https://www.python.org/downloads/)
- [Ninja Build](https://ninja-build.org/) (only needed on Windows)

For example, to install these dependencies on Ubuntu:

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
