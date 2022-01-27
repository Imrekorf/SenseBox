/**
 * @file __W_AS726X.cpp
 * @author Imre Korf
 * @brief  TSL2591 wrapper for the TSL2591 adafruit library
 * @version 0.1
 * @date 2021-11-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <Adafruit_AS726x.h>
#include "../../__W_Module/__iW_Module.h"
#include "../../Singleton/Singleton.h"

/**
 * @addtogroup STRUCT
 * @{
*/
/** @brief Struct containing the read colorspectrum values.
 */
struct ColorSpectrum {
	/** @brief Measured violet color spectrum value */
	float Violet;
	/** @brief Measured blue color spectrum value */
	float Blue;
	/** @brief Measured green color spectrum value */
	float Green;
	/** @brief Measured yellow color spectrum value */
	float Yellow;
	/** @brief Measured orange color spectrum value */
	float Orange;
	/** @brief Measured red color spectrum value */
	float Red;
};
/**@}*/

/**
 * @brief Singleton AS726X module.
 */
class __W_AS726X : public __iW_Module, public iSingleton {
private:
	/** @brief Handle to the adafruit AS726x library. */
	Adafruit_AS726x AS7262;

	/**
	 * @brief virtual implementation of the iW_Module function.
	 * Will report an error if not initialized.
	 * @return true AS7262X has been initialized.
	 * @return false AS7262X has not been initialized.
	 */
	virtual bool checkInitialized();

	// remove access to the constructor of __W_AS726X.
	__W_AS726X(){}
public:
	/**
	 * @brief Get the singleton instance of the class
	 * This function makes sure that only one instance is created and accessible during runtime.
	 * @return __W_AS726X& the handle to the singleton instance
	 */
	static __W_AS726X& getInstance(){
		static __W_AS726X Instance;	// will only be destroyed on program exit.
		return Instance;
	}	
	// Assure that only one instance can exist by removing copy and assign functions.
	__W_AS726X(__W_AS726X const&)		= delete;	// delete copy constructor.
	void operator=(__W_AS726X const&)	= delete;	// remove assignment operator.
	
	/**
	 * @brief Initializes the AS726X object. Should only be called once.
	 * This function initializes the AS726X object. It has a check build in to see if this function has already been called before. If so it will just return 0.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type init();

	/** @brief turn the Drv Led on or off. 
	 * @param B On or Off.
	*/
	void setDrvLed(bool B);
	/** @brief Set the Drv Led current. 
	 * @param V the current value.
	*/
	void setDrvCurrent(uint8_t V);
	/** @brief turn the indicate Led on or off.
	 * @param B On or Off.
	 */
	void indicateLED(bool B);
	/** @brief Set the indicate Led current. 
	 * @param V the current value.
	*/
	void setIndicateCurrent(uint8_t V);

	/**
	 * @brief Set the Gain value.
	 * @param gain the gain value.
	 */
	void setGain(uint8_t gain);
	/**
	 * @brief Set the Integration Time.
	 * @param time the integration time.
	 */
	void setIntegrationTime(uint8_t time);
	/**
	 * @brief Set the Conversion Type.
	 * 
	 * @param type The conversion Type.
	 */
	void setConversionType(uint8_t type);

	/**
	 * @brief Start a measurement.
	 */
	void startMeasurement();
	/**
	 * @brief Check if the data is ready.
	 * @return true Data is ready.
	 * @return false Data is not ready.
	 */
	bool checkDataReady();

	/**
	 * @brief Get the Measurements.
	 * @param CS The object into which the measurements should be read.
	 */
	void getMeasurements(ColorSpectrum* CS);
	/**
	 * @brief Get the Temperature value.
	 * @return uint8_t The read temperature.
	 */
	uint8_t getTemperature();
};