# *x*8K Firmware

This is the official repository containing the firmware for the 8K line of \[brand name\] mice.

# Contributing or Customizing Firmware
If you'd like to contribute to this project or customize your own 8K mouse's firmware, refer to the steps below to get started!

## Requirements
The following requirements apply only if you intend to use the development and flashing workflow provided by this repository. For example, if you wish to flash the firmware using your own devices/methods, you can do so, but you must implement those features yourself.
- You must be working in a Unix-based environment, such as a Linux distro or macOS
- You must have a working J-Link device
- You must have a USB-C cable capable of high-speed data communication

## Setup
### Install the GNU Arm Embedded toolchain
The following shell script downloads the GCC ARM Embedded toolchain (v10.3-2021.10) from the official ARM website and moves the directory containing its binaries into `~/.toolchains`. That directory is then installed to the shell's system PATH.
```sh
# WIP
```
Verify that the toolchain binaries were installed successfully by executing the following command in a new terminal.
```sh
arm-none-eabi-gcc --version
```

### Install CMake
<details>
  <summary>Steps for Linux users</summary>
  
  For Debian/Ubuntu-based systems, use the system's package manager to install CMake.
  ```sh
  sudo apt install cmake
  ```
</details>
<details>
  <summary>Steps for macOS users</summary>
  
  macOS users should use the [Homebrew](https://brew.sh/) package manager to install CMake.
  ```sh
  brew install cmake
  ```
</details>

Verify that CMake was installed successfully by executing the following command in a new terminal.
```sh
cmake --version
```

### Install J-Link Software
On the official SEGGER website, go to the [J-Link / J-Trace Downloads](https://www.segger.com/downloads/jlink/) page.

Under <ins>*J-Link Software and Documentation pack*</ins>, Download the installer or archive for your platform. If you downloaded an installer, run it and simply follow its prompts. If you downloaded an archive, extract it and add the directory containing its binaries to the shell's PATH environment variable

Verify that the J-Link software was successfully installed by executing the following command in a new terminal.
```sh
# Give unknown command line option to avoid opening interactive terminal
JLinkExe -
```

## Project Configuration and Building Firmware
This project utilizes CMake presets for both project configuration and building the firmware. You can configure the build for release or debug by telling CMake to configure for the corresponding preset.
```sh
# debug mode
cmake --preset debug-cortex-m4
# release mode
cmake --preset release-cortex-m4
```
Once configuration is complete, you can also have CMake use the build system selected by the configuration preset to build the firmware.
```sh
cmake --build --preset [preset_name]
```

## Flashing and Debugging
### SWD Interface
On your mouse's PCB, there exists an SWD interface that you can use to connect your J-Link debug probe. Other SWD debug probes should also work, but the provided *Visual Studio Code* tasks assume the SEGGER J-Link tools. Below is an image of the SWD interface.

*\[image placeholder\]*

Plug in your J-Link device and connect it to the SWD interface on your mouse, making sure to reference your probe's pinout to properly match the labelled pins.
### Serial Terminal Output
For this project, all I/O operations are handled over a UART connection. Conveniently, there is a switch on the PCB that opens a virtual serial port that you can connect a serial terminal to. Below is an image of that switch.

*\[image placeholder\]*

To see debug messages, you must first configure the project to build in debug mode. To do this, have CMake configure the project using the debug preset.
```sh
cmake --preset debug-cortex-m4
```
You must then locate the newly opened virtual serial port's device file in the `/dev` directory.
```sh
# WIP
```
Once you've found the correct device file, you can open a serial terminal with a baudrate of 115200 and have it watch that device. Below is an example using `minicom` to open a serial terminal for a device at `/dev/ttyACM0`.
```sh
sudo minicom -D /dev/ttyACM0 -b 115200
```
