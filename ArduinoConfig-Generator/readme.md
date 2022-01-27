# ArduinoConfig-Generator
This script generates a dat file containing encrypted internet connection data.
The script can be expanded on by added variables to the WiFiSettings struct and adding a way to set it in the `getWifiConfig()` function.
If new variables are added, don't forget to also implement a formatting method on the ESP32 code. 
This should be done in the MQTTClient::getSettings() function.

# compiling

compile on an msys2 configured machine by using `make`
otherwise use `make -f MakefileWindows` aslong as you have a preconfigured g++ path.

# installing

After compiling copy the MQTTSettings.dat to the appointed path on the SD card. ( by default the root folder. )
The ESP32 will handle the decrypting.