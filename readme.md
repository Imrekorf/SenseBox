# Table of contents
- [Table of contents](#table-of-contents)
- [setup guide](#setup-guide)
- [Hardware setup guide](#hardware-setup-guide)
	- [Soldering list](#soldering-list)
	- [Managing the power source](#managing-the-power-source)
	- [Connecting the sensors](#connecting-the-sensors)
		- [Datasheets from the currently used sensors](#datasheets-from-the-currently-used-sensors)
		- [Schematics usage](#schematics-usage)
- [Programming the PCB](#programming-the-pcb)
- [Troubleshooting errors](#troubleshooting-errors)
	- [RTC](#rtc)
	- [SD](#sd)
	- [Sensor initialisation errors](#sensor-initialisation-errors)
- [Correct Installation check](#correct-installation-check)

# setup guide

1. Clone this github repo
2. Generate a MQTTSettings.dat file using the ArduinoConfig-Generator script.
3. Move generated MQTTSettings.dat to SD card's root folder.
4. Power the PCB.

# Hardware setup guide

## Soldering list
1. Solder the DC barreljack
2. Solder the BT1 Battery holder.[^1]
3. Solder the Laser dust sensor connector pins.[^1]
4. Solder the VR5V and VR5V jumpers.

[^1]: Currently the Laser Dust sensor connectors and Battery holder have not yet arrived so that sensor and battery holder can not be connected yet.

## Managing the power source
Depending on the circumstance you might want to power the PCB from the USB connection.<br>
To do this, **first**, you must make sure that VR5V and VR3V3 solder pads are not soldered. This can be identified by seeing 2 small pads on both the solder pads.<br>
Secondly you must solder the USB3V3 and USB5V to provide power to the board from the USB.<br>
If the DC barreljack is used you must **desolder** the USB3V3 and USB5V and then solder the VR3V3 and VR5V.<br>
<br>
To power the RTC you must either solder on the battery holder on the BT1 footprint. Or you can solder a wire from the VR3V3 or USB3V3, whichever is currently providing the 3V3 power, to the 3V3 pad of the Battery holder pads. See the board schematic included in the Schematics folder for more details.<br><br>

## Connecting the sensors
The sensors should be connected to the different JST headers provided on the board. To do this you need male `JST-PH 2.0 4 pin connectors`. 
These can be ordered from [vanallesenmeer.nl](https://www.vanallesenmeer.nl/JST-2.0-PH-4pin-set-recht-met-kabel-30cm) for €0.95.
These cables' other end can then be soldered to the sensors. Currently i2c compatible devices are wired as followed from left to right:
1. SCL : black
2. SDA : red
3. 3v3 : white
4. GND : yellow.

Other sensors' connection diagrams should be taken from the provided schematics.

### Datasheets from the currently used sensors
- [SCD30-datasheet](https://docs.rs-online.com/2d49/0900766b816b6f9d.pdf) & [SCD30-interface descriptor](https://docs.rs-online.com/ba78/0900766b816b6f9c.pdf)
- [Ambimate datasheet](https://docs.rs-online.com/6304/A700000006497831.pdf), [Product specifications](https://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=108-133092&DocType=SS&DocLang=EN), [Application Specifications](https://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Specification+Or+Standard%7F114-133092%7FB%7Fpdf%7FEnglish%7FENG_SS_114-133092_B.pdf%7FN-A), [customer drawings](https://eu.mouser.com/datasheet/2/418/7/ENG_CD_2314291_A2-2032749.pdf)
- [TSL2591](https://learn.adafruit.com/adafruit-tsl2591/downloads)
- [AS7262](https://learn.adafruit.com/adafruit-as7262-6-channel-visible-light-sensor/overview)
- [SM-UART-04L](https://www.farnell.com/datasheets/2756490.pdf)
- [MIX8410](https://wiki.seeedstudio.com/Grove-Gas_Sensor-O2-MIX8410/)
- [MAX4466](https://datasheets.maximintegrated.com/en/ds/MAX4465-MAX4469.pdf)
- [PCF8563](https://www.nxp.com/docs/en/data-sheet/PCF8563.pdf)

Any other datasheets needed are included in the EAGLE library. Check the description of the different components to find these datasheets.


### Schematics usage
The .sch and .brd file contain the electrical diagrams and PCB board layout respectively. The pdf contains a quick summary of the .sch file.
The .sch and .brd files can be loaded into the Autocad EAGLE software to make changes and view other information.
The .csv files contain JLCPCB part numbers and placements. These are used to order the right SMD components on the board.

# Programming the PCB
To program the ESP32 on the PCB, provide a power source, either through the USB or the barrel jack depending on the currently soldered connections.
Then load the code into the arduino ide by opening the .ino file. You then need to select the ESP32 dev module as your board in the Tools section.
If this is not already present follow the following [tutorial](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) to add support for these boards.
Select the correct port in the arduino ide and upload the code.
**important:** when the "connecting ...." prompt comes up, press and hold the RST and BOOT buttons, then release the RST button. if the prompt stops printing `.` or `-`'s then release the BOOT button.

# Troubleshooting errors
The most common errors in the serial output will be that either the RTC, SD or other sensors are not connected. Currently the program does not stop at these errors which could lead to a SYS_RST error message from the ESP32.
## RTC
If the RTC is not working there is either a short circuit on the i2c bus or the RTC does not have power. The RTC is the first module to be initialized in the system for logging purposes, so this has some major consequences when it's not working.

## SD
The most common SD card error is that it is not plugged in. Which can be solved by plugging in the SD card, or replugging the SD card and restarting the system when it was plugged in.
The provided SD card should be flashed with an FAT32 format to make sure the ESP32 can write to it.

## Sensor initialisation errors
If problems arise with the sensors not initializing it is likely due to bad wiring or not being connected at all.

# Correct Installation check
If the installation and connections are all correct you should see the following popping up in the serial monitor after a reset.

```
[14:39:31] [Info]: AmbiMate Firmware version 2.8
[14:39:32] [Info]: AS7262 initialized
[14:39:32] [Info]: SM_UART_4L found and initialized.
[14:39:32] [Info]: SCD30 initialized.
[14:39:32] [Info]: Found a TSL2591 sensor
[14:39:32] [Info]: TSL2591 bootup
------------------------------------
Gain:         25x (Medium)
Timing:       300 ms
------------------------------------

[14:39:33] [Info]: Connecting to WiFi .......
Connencted to wiFi with ip: 192.168.178.53
[14:39:38] [Info]: 
Starting connection to server...
[14:39:38] [Info]: The client SenseBox_test connects to the public mqtt broker
[54:39:40] [Info]: Mqtt broker connected
```

Followed by Info logs containing sensor data.
This data should also be partially tracable to the IoT platform.