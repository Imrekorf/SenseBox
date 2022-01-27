/**
 * @file Sbox.h
 * @author Imre Korf
 * @brief Contains the highest level SenseBox wrapper class
 * @version 0.1
 * @date 2021-11-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

/*
SCD30: CO2
MIX8410: O2
TSL2591: LI
AS7262: Spectrum
2314291-2: VOC + extra
MAX4466: Sound
SM-UART-04L: Fijnstof
*/


/*
All properties:
O2
CO2
Humidity
Temp
Light Intensity (LUX)
Light Spectrum (6band and intensity)
VOC
eCO2
dB + Hz?
Movement (flag)
Dust
*/

#include <Arduino.h>

#include "../Wrappers/Sensors/Ambimate/__W_Ambimate.h"
#include "../Wrappers/Sensors/AS7262/__W_AS726X.h"
#include "../Wrappers/Sensors/DustSensor/__W_SMUART_4L.h"
#include "../Wrappers/Sensors/MAX4466/__W_MAX4466.h"
#include "../Wrappers/Sensors/MIX8410/__W_MIX8410.h"
#include "../Wrappers/Sensors/SCD30/__W_SCD30.h"
#include "../Wrappers/Sensors/TSL2591/__W_TSL2591.h"
#include "../Wrappers/RTC/__W_RTC.h"

/**
 * @brief SBox class containing handles to every sensor on the PCB. 
 */
class SBox {
private:

	/** Ambimate Handle. */
	__W_Ambimate 	*Ambimate;
	/** AS726X Handle. */
	__W_AS726X 	 	*AS7262;
	/** SM_UART_4L Handle. */
	__W_SM_UART_4L 	*LDS;
	/** MAX4466 Handle. */
	__W_MAX4466  	*MAX4466;
	/** MIX8410 Handle. */
	__W_MIX8410  	*MIX8410;
	/** SCD30 Handle. */
	__W_SCD30 	 	*SCD30;
	/** TSL2591 Handle. */
	__W_TSL2591  	*TSL2591;
	/** RTC Handle. */
	__W_RTC			*RTC;

public:
	/**
	 * @brief Initializes the SBox object. Should only be called once.
	 * This function initializes the SBox object. It has a check build in to see if this function has already been called before. If so it will just return 0.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type init();

	/**
	 * @brief Get the a date_time struct.
	 * 
	 * @return RTC_DATE_TIME struct containing current date and time.
	 */
	RTC_DATE_TIME getTime();

	/**
	 * @brief Get the Ambimate Data.
	 * 
	 * @return AmbimateData struct containing the read ambimate data.
	 */
	AmbimateData getAmbimateData();
	/**
	 * @brief Get the Color Spectrum data.
	 * 
	 * @param CS ColorSpectrum struct buffer.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type	 getColorSpectrum(ColorSpectrum& CS);
	/**
	 * @brief Get the Laser Dust Sensor data.
	 * 
	 * @param Buffer PM25_AQI_Data struct buffer.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type 	getLDSData(PM25_AQI_Data& Buffer);
	/**
	 * @brief Get the Max4466 data.
	 * 
	 * @return int ADC value of the Max4466 data.
	 */
	int 		 getMax4466();
	/**
	 * @brief Get the O2 value.
	 * 
	 * @return float the read O2 value.
	 */
	float 		 getO2();
	/**
	 * @brief Get the SCD30 data.
	 * 
	 * @return SCD30_DATA struct containing the read SCD30 data.
	 */
	SCD30_DATA	 getSCD30Data();
	/**
	 * @brief Get the TSL2591 data.
	 * 
	 * @return TSL2591_DATA struct containing the TSL2591 data.
	 */
	TSL2591_DATA getTSL2591Data();
};