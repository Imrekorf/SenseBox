/**
 * @file __iW_Module.h
 * @author Imre Korf
 * @brief Module interface for intergrating sensors and other modules
 * @version 0.1
 * @date 2021-11-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <Arduino.h>
#include "../../Defines/Defines.h"

/**
 * @brief Interface for a hardware module
 * Interface for a hardware module
 * hardware module should be implemented as a singleton to ensure 
 * that there won't be two processes accessing the same hardware at the same time
 * Singleton format: https://stackoverflow.com/a/1008289
 */
class __iW_Module {
protected:
	/**
	 * @brief The Initialized flag is used to keep track of the state of the Module.
	 * 
	 */
	bool Initialized = false;
	/**
	 * @brief Checks if the Init function has been called
	 * 
	 * some sensors will have an initialisation that needs to be completed first.
	 * Internally the functions of those sensors should start with a check if the sensor is initialized and if it is not immediatly return.
	 * @return true the init function has been called and exited successfully
	 * @return false the init function has not been called or exited with an error.
	 */
	virtual bool checkInitialized(){return Initialized;}

public:
	/**
	 * @brief a standard initialisation method
	 * 
	 * @return ERR_Type returns 0 on success, 1 on failure
	 * @see ERR_Type
	 */
	virtual ERR_Type init() = 0;
};