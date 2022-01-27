/**
 * @file __W_TSL2591.h
 * @author Imre Korf
 * @brief TSL2591 wrapper for the TSL2591 adafruit library
 * @version 0.1
 * @date 2021-11-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>

#include "../../__W_Module/__iW_Module.h"
#include "../../Singleton/Singleton.h"

/**
 * @addtogroup STRUCT
 * @{
 */

/** @brief Struct containing the TSL2591 data. */
struct TSL2591_DATA {
	/** @brief Lux values of visible spectrum range. */
	uint16_t visible;
	/** @brief Lux values of infra red spectrum range. */
	uint16_t ir;
	/** @brief Lux values of full spectrum range. */
	uint16_t full;
};
/**@}*/

/**
 * @brief Singleton TSL2591 module.
 */
class __W_TSL2591 : public __iW_Module, public iSingleton {
private:
	/** @brief Handle to the adafruit TSL2591 library. */
	Adafruit_TSL2591 tsl; 

	/**
	 * @brief virtual implementation of the iW_Module function.
	 * Will report an error if not initialized.
	 * @return true TSL2591 has been initialized.
	 * @return false TSL2591 has not been initialized.
	 */
	virtual bool checkInitialized();
	// remove access to the constructor of __W_TSL2591.
	__W_TSL2591() : tsl(2591){} // pass in a number for the sensor identifier (for your use later).
public:
	/**
	 * @brief Get the singleton instance of the class
	 * This function makes sure that only one instance is created and accessible during runtime.
	 * @return __W_TSL2591& the handle to the singleton instance
	 */
	static __W_TSL2591& getInstance(){
		static __W_TSL2591 Instance;	// will only be destroyed on program exit.
		return Instance;
	}
	// Assure that only one instance can exist by removing copy and assign functions.
	__W_TSL2591(__W_TSL2591 const&)			= delete;	// delete copy constructor.
	void operator=(__W_TSL2591 const&)	= delete;	// remove assignment operator.
	
	/**
	 * @brief Initializes the TSL2591 object. Should only be called once.
	 * This function initializes the TSL2591 object. It has a check build in to see if this function has already been called before. If so it will just return 0.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type init();

	/**
	 * @brief Displays the sensor details to the SD card and Serial.
	 * 
	 */
	void displaySensorDetails();

	/**
	 * @brief Get the Luminosity value
	 * 
	 * @param spectrum Spectrum to be read. other values are: TSL2591_FULLSPECTRUM or TSL2591_INFRARED.
	 * @return uint16_t raw 16-bit ADC value
	 */
	uint16_t getLuminosity(uint8_t spectrum = TSL2591_VISIBLE);
	/**
	 * @brief Read the full luminosity.
	 * 
	 * @return TSL2591_DATA Struct containing all luminosity data.
	 */
	TSL2591_DATA getFullLuminosity();
	
	/**
    @brief  Calculates the visible Lux based on the two light sensors
    @param  full Data from channel 0 (IR+Visible)
    @param  ir Data from channel 1 (IR)
    @returns Lux, based on AMS coefficients (or < 0 if overflow)
	*/
	float getLux(uint16_t full, uint16_t ir);
};