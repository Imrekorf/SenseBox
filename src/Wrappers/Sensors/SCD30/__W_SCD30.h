/**
 * @file __W_SCD30.h
 * @author Imre Korf
 * @brief SCD30 wrapper for the SCD30 sensor
 * @version 0.1
 * @date 2021-11-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <Wire.h>
#include <SparkFun_SCD30_Arduino_Library.h>
#include "../../__W_Module/__iW_Module.h"
#include "../../Singleton/Singleton.h"

/**
 * @addtogroup STRUCT
 * @{
 */
/** @brief Struct containg the SCD30 Data */
struct SCD30_DATA {
	/** CO2 ppm value */
	uint16_t CO2;
	/** Temperature value */
	float Temperature;
	/** Humidity value */
	float Humidity;
};
/**@}*/

/**
 * @brief Singleton SCD30 module.
 */
class __W_SCD30 : public __iW_Module, public iSingleton {
private:	
	/** @brief Handle to the SCD30 library. */
	SCD30 airSensor;
	
	/**
	 * @brief boolean value indicating if the sensor should autocalibrate.
	 */
	bool autoCalibrate = false;
	/**
	 * @brief virtual implementation of the iW_Module function.
	 * Will report an error if not initialized.
	 * @return true SCD30 has been initialized.
	 * @return false SCD30 has not been initialized.
	 */
	virtual bool checkInitialized();

	// remove access to the constructor of __W_SCD30
	__W_SCD30(){}
public:
	/**
	 * @brief Get the singleton instance of the class
	 * This function makes sure that only one instance is created and accessible during runtime.
	 * @return __W_SCD30& the handle to the singleton instance
	 */
	static __W_SCD30& getInstance(){
		static __W_SCD30 Instance;	// will only be destroyed on program exit.
		return Instance;
	}
	// Assure that only one instance can exist by removing copy and assign functions.
	__W_SCD30(__W_SCD30 const&)			= delete;	// delete copy constructor.
	void operator=(__W_SCD30 const&)	= delete;	// remove assignment operator.

	/**
	 * @brief Reads the SCD30 sensor.
	 * 
	 * @return SCD30_DATA Struct containing the read SCD30 Data.
	 */
	SCD30_DATA read();

	/**
	 * @brief Init function if autoCalibrate is set to true, this will activate the sensors autocalibrate feature.
	 * Please see section 1.3.6 of the SCD30 datasheet:
 	   "When activated for the first time a
		period of minimum 7 days is needed so that the algorithm can find its initial parameter set for ASC. The sensor has to be exposed
		to fresh air for at least 1 hour every day. Also during that period, the sensor may not be disconnected from the power supply,
		otherwise the procedure to find calibration parameters is aborted and has to be restarted from the beginning. The successfully
		calculated parameters are stored in non-volatile memory of the SCD30 having the effect that after a restart the previously found
		parameters for ASC are still present. "

	 * @param autoCalibrate turns on auto calibrate.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code
	 */
	ERR_Type init(bool autoCalibrate);
	
	/**
	 * @brief Initializes the SCD30 object. Should only be called once.
	 * This function initializes the SCD30 object. It has a check build in to see if this function has already been called before. If so it will just return 0.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type init();

	/**
	 * @brief Checks if the data is available.
	 * 
	 * @return true Data is ready.
	 * @return false Data is not ready.
	 */
	bool dataAvailable();

	/**
	 * @brief Returns the read CO2 ppm value.
	 * 
	 * @return uint16_t the CO2 in ppm.
	 */
	uint16_t getCO2();
	/**
	 * @brief Get the Temperature value.
	 * 
	 * @return float The temperature.
	 */
	float getTemperature();
	/**
	 * @brief Get the Humidity level.
	 * 
	 * @return float The Humidity level.
	 */
	float getHumidity();

	// Options:
	/**
	 * @brief Set the Measurements Interval.
	 * Change number of seconds between measurements: 2 to 1800 (30 minutes), stored in non-volatile memory of SCD30.
	 * @param seconds 
	 */
	void setMeasurementsInterval(uint16_t seconds);
	
	/**
	 * @brief Get the Measurements Interval.
	 * 
	 * @param seconds object into which the value should be read.
	 * @return true get successfull.
	 * @return false get failure.
	 */
	bool getMeasurementsInterval(uint16_t& seconds);

	/**
	 * @brief Set the Altitude Compensation 
	 * Set altitude of the sensor in m, stored in non-volatile memory of SCD30.
	 * @param altitude Altitude of the sensor in m.
	 */
	void setAltitudeCompensation(uint16_t altitude);

	/**
	 * @brief Get the Altitude Compensation object.
	 * 
	 * @param altitude object into which the value should be read.
	 * @return true get successfull.
	 * @return false get failure.
	 */
	bool getAltitudeCompensation(uint16_t& altitude);

	/**
	 * @brief Set the Ambient Pressure.
	 * Current ambient pressure in mBar: 700 to 1200, will overwrite altitude compensation.
	 * @param Pressure Current ambient pressure in mBar: 700 to 1200
	 */
	void setAmbientPressure(uint16_t Pressure);

	/**
	 * @brief Set the Temperature Offset
	 * Optionally we can set temperature offset to °C, stored in non-volatile memory of SCD30.
	 * @param offset temperature offset.
	 */
	void setTemperatureOffset(float offset);

	/**
	 * @brief Get the Temperature Offset.
	 * 
	 * @param offset object into which the value should be read.
	 * @return true get successfull.
	 * @return false get failure.
	 */
	bool getTemperatureOffset(float& offset);

	/**
	 * @brief Get the Auto Self Calibration value.
	 * 
	 * @return true autoselfcalibration is on.
	 * @return false autoselfcalibration is off.
	 */
	bool getAutoSelfCalibration();

	/**
	 * @brief Get the Forced Recalibration value.
	 * 
	 * @param settingVal object into which the value should be read.
	 * @return true get successfull.
	 * @return false get failure.
	 */
	bool getForcedRecalibration(uint16_t& settingVal);

	/**
	 * @brief Get the Firmware Version.
	 * 
	 * @param settingVal object into which the value should be read.
	 * @return true get successfull.
	 * @return false get failure.
	 */
	bool getFirmwareVersion(uint16_t& settingVal);
};