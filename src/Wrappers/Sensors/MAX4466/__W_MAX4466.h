/**
 * @file __W_MAX4466.h
 * @author Imre Korf
 * @brief MAX4466 wrapper for the MAX4466 sensor
 * @version 0.1
 * @date 2021-11-23
 * 
 * source: https://blog.yavilevich.com/2016/08/arduino-sound-level-meter-and-spectrum-analyzer/
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include "../../__W_Module/__iW_Module.h"
#include "../../Singleton/Singleton.h"

/**
 * @brief Singleton MAX4466 module.
 */
class __W_MAX4466 : public __iW_Module, public iSingleton {
private:	
	/**
	 * @brief virtual implementation of the iW_Module function.
	 * Will report an error if not initialized.
	 * @return true MAX4466 has been initialized.
	 * @return false MAX4466 has not been initialized.
	 */
	virtual bool checkInitialized();
	
	// remove access to the constructor of __W_MAX4466.
	__W_MAX4466(){}
public:
	/**
	 * @brief Get the singleton instance of the class
	 * This function makes sure that only one instance is created and accessible during runtime.
	 * @return __W_MAX4466& the handle to the singleton instance
	 */
	static __W_MAX4466& getInstance(){
		static __W_MAX4466 Instance;	// will only be destroyed on program exit.
		return Instance;
	}
	// Assure that only one instance can exist by removing copy and assign functions.
	__W_MAX4466(__W_MAX4466 const&)		= delete;	// delete copy constructor.
	void operator=(__W_MAX4466 const&)	= delete;	// remove assignment operator.
	
	/**
	 * @brief Initializes the MAX4466 object. Should only be called once.
	 * This function initializes the MAX4466 object. It has a check build in to see if this function has already been called before. If so it will just return 0.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type init();

	/**
	 * @brief Reads the MAX4466 analog value.
	 * 
	 * @return int The ADC converted value.
	 */
	int read();

};