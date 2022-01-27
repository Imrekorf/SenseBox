/**
 * @file __W_SMUART_4L.h
 * @author Imre Korf
 * @brief SM_UART_4L wrapper for the PM25AQI adafruit library
 * @version 0.1
 * @date 2021-11-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <Adafruit_PM25AQI.h>
#include "../../__W_Module/__iW_Module.h"
#include "../../Singleton/Singleton.h"

/**
 * @brief Singleton LDS module.
 */
class __W_SM_UART_4L : public __iW_Module, public iSingleton {
private:
	/** @brief Handle to the adafruit PM25AQI library. */
	Adafruit_PM25AQI aqi;
	
	/**
	 * @brief virtual implementation of the iW_Module function.
	 * Will report an error if not initialized.
	 * @return true LDS has been initialized.
	 * @return false LDS has not been initialized.
	 */
	virtual bool checkInitialized();
	
	// remove access to the constructor of __W_SM_UART_4L.
	__W_SM_UART_4L(){}
public:
	/**
	 * @brief Get the singleton instance of the class
	 * This function makes sure that only one instance is created and accessible during runtime.
	 * @return __W_SM_UART_4L& the handle to the singleton instance
	 */
	static __W_SM_UART_4L& getInstance(){
		static __W_SM_UART_4L Instance;	// will only be destroyed on program exit.
		return Instance;
	}
	// Assure that only one instance can exist by removing copy and assign functions.
	__W_SM_UART_4L(__W_SM_UART_4L const&)	= delete;	// delete copy constructor.
	void operator=(__W_SM_UART_4L const&)	= delete;	// remove assignment operator.
	
	/**
	 * @brief Initializes the LDS object. Should only be called once.
	 * This function initializes the LDS object. It has a check build in to see if this function has already been called before. If so it will just return 0.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type init();

	/**
	 * @brief Reads the LDS sensor data into the data object.
	 * 
	 * @param data The PM25_AQI_Data object into which the data should be read.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type read(PM25_AQI_Data& data);
};