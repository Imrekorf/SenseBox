#include "__W_Ambimate.h"
#include <Arduino.h>
#include "../../../Logger/Logger.h"

bool __W_Ambimate::checkInitialized(){
    if(Initialized){
        return 0;
    }
    else{
        Logger::getInstance().println("Ambimate is not initialized!", LogLevel::Error);
        return 1;
    }
}

ERR_Type __W_Ambimate::init() {
	if(Initialized){return ALREADY_INITIALIZED;}	// return if already initialized;

	// Data and basic information are acquired from the module
	Wire.beginTransmission(0x2A); // transmit to device
	Wire.write(byte(0x80));       // sends instruction to read firmware version
	if(Wire.endTransmission()){   // stop transmitting
		Logger::getInstance().print("Ambimate I2C Error: ", LogLevel::Error); 
		Logger::getInstance().println(String(Wire.getErrorText(Wire.lastError())), LogLevel::Error);
		return AMBI_I2C_INIT_ERR;
	}      
	Wire.requestFrom(0x2A, 1);    // request byte from slave device
	uint8_t fw_ver = Wire.read(); // receive a byte

	Wire.beginTransmission(0x2A); // transmit to device
	Wire.write(byte(0x81));       // sends instruction to read firmware subversion
	Wire.endTransmission();       // stop transmitting
	Wire.requestFrom(0x2A, 1);    // request byte from slave device
	uint8_t fw_sub_ver = Wire.read(); // receive a byte

	Wire.beginTransmission(0x2A); // transmit to device
	Wire.write(byte(0x82));       // sends instruction to read optional sensors byte
	Wire.endTransmission();       // stop transmitting
	Wire.requestFrom(0x2A, 1);    // request byte from slave device
	opt_sensors = Wire.read();    // receive a byte

	//TODO: check if this delay is neccessary
	delay(1000);

	// debug info

	//If device contains additional CO2 and audio sensor, it is indicated here
	Logger::getInstance().print("AmbiMate sensors: 4 core", LogLevel::Info);
	if (opt_sensors & 0x01)
		Logger::getInstance().print(" + CO2", LogLevel::Info);
	if (opt_sensors & 0x04)
		Logger::getInstance().print(" + Audio", LogLevel::Info);
	Logger::getInstance().println(" ", LogLevel::Info);

	Logger::getInstance().print("AmbiMate Firmware version ", LogLevel::Info);
	Logger::getInstance().print(fw_ver, LogLevel::Info);
	Logger::getInstance().print(".", LogLevel::Info);
	Logger::getInstance().println(fw_sub_ver, LogLevel::Info);

	Initialized = true;
	return SUCCESS;
}

AmbimateData __W_Ambimate::read(){
	AmbimateData AD;
	if(checkInitialized()){return AD;} // don't act on to the hardware if not properly intialized;

	// All sensors except the CO2 sensor are scanned in response to this command
	Wire.beginTransmission(0x2A); // transmit to device
	// Device address is specified in datasheet
	Wire.write(byte(0xC0));       // sends instruction to read sensors in next byte
	Wire.write(0xFF);             // 0xFF indicates to read all connected sensors
	Wire.endTransmission();       // stop transmitting
	// Delay to make sure all sensors are scanned by the AmbiMate
	delay(100);

	Wire.beginTransmission(0x2A); // transmit to device
	Wire.write(byte(0x00));       // sends instruction to read sensors in next byte
	Wire.endTransmission();       // stop transmitting
	Wire.requestFrom(0x2A, 15);    // request 6 bytes from slave device

	// Acquire the Raw Data
	uint16_t i = 0;
	uint8_t buf[20];
	while (Wire.available()) { // slave may send less than requested
		buf[i] = Wire.read(); // receive a byte as character and store in buffer
		i++;
	}


	// convert the raw data to engineering units, see application spec for more information 
	AD.status = buf[0];
	AD.temperatureC = (buf[1] * 256.0 + buf[2]) / 10.0;
	AD.Humidity = (buf[3] * 256.0 + buf[4]) / 10.0;
	AD.light = (buf[5] * 256.0 + buf[6]);
	AD.audio = (buf[7] * 256.0 + buf[8]);
	AD.batVolts = ((buf[9] * 256.0 + buf[10]) / 1024.0) * (3.3 / 0.330);
	AD.eco2_ppm = (buf[11] * 256.0 + buf[12]);
	AD.voc_ppm = (buf[13] * 256.0 + buf[14]);

	return AD;
}

uint8_t __W_Ambimate::get_opt_sensors(){
	if(checkInitialized()){return 0;} // don't act on to the hardware if not properly intialized;
	return opt_sensors;
}