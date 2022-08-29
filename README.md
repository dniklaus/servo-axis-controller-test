# servo-axis-controller-test
Wiring / Arduino based Embedded Application to test Servo Axis control process class Axis, built on [PlatformIO](http://platformio.org "Cross-platform build system").

The project has been created for and tested on an Arduino Mega 2560 controller module.

## Toolchain
[PlatformIO](http://platformio.org "Cross-platform build system") is the ideal foundation when developing IoT devices. It supports cross-platform development for many different controller boards widely used in the maker field and also for industrial applications.

### Installation
#### Python 3.7

**Windows**

* download Python 3.7.x or later from: https://www.python.org/downloads/
* select destination directory: keep default
* make sure python.exe is added to path

**Linux**

Install Python 3.7 or later using your package manager.

#### PlatformIO
Install PlatformIO using the Python Package Manager
(see also http://docs.platformio.org/en/latest/installation.html#python-package-manager):

This step can be omitted when directly working with Visual Studio Code (see below).

* in a cmd shell enter: `pip install -U platformio`
* upgrade pip, in the cmd shell enter: `python -m pip install --upgrade pip`
* upgrade PlatformIO, in the cmd shell enter: `pio upgrade`

### Working with Visual Studio Code

#### How to install PlatformIO IDE in VSCode

  1. Open VSCode 
  2. In VSCode open the Extensions tab (or just press Ctrl+Shift+x) and search for the extension "PlatformIO IDE" and click on Install

#### Open project in VSCode

  1. Create a directory where your code project shall be cloned into. E.g. `C:\git\pio-prj`
  2. Clone this repository into the folder you created before: 
     `git clone git@github.com:dniklaus/servo-axis-controller-test.git` (ssh) or
     `git clone https://github.com/dniklaus/servo-axis-controller-test.git` (ssl)
  3. Open the just cloned project folder, i.e. `C:\git\pio-prj\servo-axis-controller-test` with VSCode (Menu Edit > Open Folder or press Ctrl+K Ctrl+O)

## Connect Terminal Emulation
In order to test and run the CLI commands, a terminal emulation program shall be used. The one giving you the best experience will be the [HTerm](http://www.der-hammer.info/terminal/). 
Load the _hterm-com9.cfg_ file to configure HTerm properly. Alter the COM9 accordingly to the one that has been selected on your computer.

## Debug Features
### Debug CLI Commands
#### Command Tree
     dbg                      Debug CLI root node
       tr                     Debug Trace Port config access
         heap                 Particular Trace Port (heap: see below in chapter Trace Port)
           out                Trace Output config access for the current trace port
             get              Show the assigned Trace Output
             set <outName>    Set a particular Trace Output name
             list             Show all available Trace Output names (and the currently selected)
           lvl                Trace Level config access for the current trace port
             get              Show the current trace level
             set <level>      Set a particular trace level
             list             Show all available trace levels (and the currenntly selected)
         ax0
         ax1
         ax2
         ax3
         ax4
       init
       ax0
         set <angle> <speed>
       ax1
         set <angle> <speed>
       ax2
         set <angle> <speed>
       ax3
         set <angle> <speed>
       ax4
         set <angle> <speed>

#### Example commands
* `dbg tr heap lvl set debug`
* `dbg ax0 set 40 250`
* `dbg ax1 set -90 120`

### Trace Port
|Trace Port|default level|functionality|
|----------|-------------|:------------|
|heap|info|if set to debug level: automatically print free heap memory [bytes], every 10 seconds|
|ax0|info||
|ax1|info||
|ax2|info||
|ax3|info||
|ax4|info||

## Library Usage
This chapter lists all the libraries this project is using.

### PlatformIO Libraries
|ID|Name|URL|Description|
|:--|:-------|:----------------|:-----------------------|
||SpinTimer|https://github.com/dniklaus/spin-timer|Universal recurring or non-recurring Timer|
||Dbg-Trace|https://github.com/ERNICommunity/dbg-trace|Debug Trace component for Embedded Applications - Debug and Trace Log message system based on trace ports with adjustable levels|
||Debug-Cli|https://github.com/ERNICommunity/debug-cli|Debug CLI for Embedded Applications - Command Line  Interface for debugging and testing based on object oriented tree structure|


### Homebrew Libraries
|Name|URL|Description|
|:------|:---------------------|:-------------------------------|
|Arduino-SerialCommand|https://github.com/dniklaus/Arduino-SerialCommand|A Wiring/Arduino library to tokenize and parse commands received over a serial port|
|wiring-app-debug|https://github.com/dniklaus/wiring-app-debug#2.0.1|Wiring application debug setup|
|arduino-utils-mem|https://github.com/dniklaus/arduino-utils-mem#2.1.0|Arduino Memory Utilities|
