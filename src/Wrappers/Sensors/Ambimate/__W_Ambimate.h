/**
 * @file __W_Ambimate.h
 * @author Imre Korf
 * @brief Ambimate wrapper for the Ambimate sensor
 * @version 0.1
 * @date 2021-11-29
 * 
 * source: https://maker.pro/esp8266/tutorial/how-to-program-esp32-with-arduino-ide-and-connect-the-ambimate-ms4-multi-sensor-module
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <Wire.h>
#include "../../__W_Module/__iW_Module.h"
#include "../../Singleton/Singleton.h"

/**
 * @addtogroup STRUCT
 * @{
 */
/** @brief struct containing the ambimate data.
 */
struct AmbimateData {
	union {
		/** the status full byte. */
		uint8_t status;
		struct {
			/** True when an motion event has occured. */
			uint8_t MOT_EVENT : 1; // motion event.
			/** True when an Audio event has occured. */
			uint8_t AUD_EVENT : 1; // audio event.
			uint8_t 		  : 5; // padding.
			/** True when a PIR event has occured. */
			uint8_t PIR_EVENT : 1; // PIR event.
		};
	};
	/** Temperature in C. */
	float temperatureC;
	/** Humidity. */
	float Humidity;
	/** Battery voltage. */
	float batVolts;
	/** Light level. */
	uint16_t light;
	/** Audio level. */
	uint16_t audio;
	/** equivalent CO2 ppm. */
	uint16_t eco2_ppm;
	/** VOC ppm. */
	uint16_t voc_ppm;
};
/**@}*/

/**
 * @brief Singleton ambimate module.
 */
class __W_Ambimate : public __iW_Module, public iSingleton {
private:
	/**
	 * @brief contains flags indicating which option senors are present.
	 */
	uint8_t opt_sensors;

	/**
	 * @brief virtual implementation of the iW_Module function.
	 * Will report an error if not initialized.
	 * @return true Ambimate has been initialized.
	 * @return false Ambimate has not been initialized.
	 */
	virtual bool checkInitialized();
	
	// remove access to the constructor of __W_Ambimate.
	__W_Ambimate(){}

public:
	/**
	 * @brief Get the singleton instance of the class
	 * This function makes sure that only one instance is created and accessible during runtime.
	 * @return __W_Ambimate& the handle to the singleton instance
	 */
	static __W_Ambimate& getInstance(){
		static __W_Ambimate Instance;	// will only be destroyed on program exit.
		return Instance;
	}
	// Assure that only one instance can exist by removing copy and assign functions.
	__W_Ambimate(__W_Ambimate const&)			= delete;	// delete copy constructor.
	void operator=(__W_Ambimate const&)	= delete;	// remove assignment operator.

	/**
	 * @brief Initializes the Ambimate object. Should only be called once.
	 * This function initializes the Ambimate object. It has a check build in to see if this function has already been called before. If so it will just return 0.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type init();

	/**
	 * @brief Reads the Ambimate sensor's data into the AmbimateData struct.
	 * eCO2/VOC sensor is only updated in AmbiMate every 60 seconds.
	 * @return AmbimateData the struct containing the read data.
	 */
	AmbimateData read();

	/**
	 * @brief Get the opt sensors flags.
	 * 
	 * @return uint8_t byte containing the optional sensor flags.
	 */
	uint8_t get_opt_sensors();
};