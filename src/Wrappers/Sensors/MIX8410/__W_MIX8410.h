/**
 * @file __W_MIX8410.h
 * @author Imre Korf
 * @brief MIX8410 wrapper for the MIX8410 sensor
 * @version 0.1
 * @date 2021-11-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include "../../__W_Module/__iW_Module.h"
#include "../../Singleton/Singleton.h"

/**
 * @brief Singleton MIX8410 module.
 */
class __W_MIX8410 : public __iW_Module, public iSingleton {

private:	
	/**
	 * @brief The Voltage reference of the ADC.
	 */
	const float VRefer = 3.3;
	/**
	 * @brief The Pin onto which the module is connected.
	 */
	const int pinAdc   = A0;		

	// remove access to the constructor of __W_MIX8410.
	__W_MIX8410(){}
public:
	/**
	 * @brief Get the singleton instance of the class
	 * This function makes sure that only one instance is created and accessible during runtime.
	 * @return __W_MIX8410& the handle to the singleton instance
	 */
	static __W_MIX8410& getInstance(){
		static __W_MIX8410 Instance;	// will only be destroyed on program exit.
		return Instance;
	}
	// Assure that only one instance can exist by removing copy and assign functions.
	__W_MIX8410(__W_MIX8410 const&)		= delete;	// delete copy constructor.
	void operator=(__W_MIX8410 const&)	= delete;	// remove assignment operator.

	/**
	 * @brief Initializes the MIX8410 object. Should only be called once.
	 * This function initializes the MIX8410 object. It has a check build in to see if this function has already been called before. If so it will just return 0.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type init(){return SUCCESS;}
	/**
	 * @brief Converted ADC value into O2 percentage.
	 * 
	 * @return float The Percentage of Oxygen in the air.
	 */
	float readConcentration();
	/**
	 * @brief Reads the ADC value and converts it into a voltage value.
	 * 
	 * @return float The read ADC value.
	 */
	float readO2Vout();
};