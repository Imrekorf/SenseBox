/**
 * @file __W_RTC.h
 * @author Imre Korf
 * @brief Wrapper for the PC8563 RTC
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include "../__W_Module/__iW_Module.h"
#include "../Singleton/Singleton.h"

#include <RTC.h>

/**
 * @defgroup STRUCT Global structures
 * This list details the global structures used in the project.
 * @{
 */
/**
 * @brief Struct containing the date and time of when the read() function was called.
 */
struct RTC_DATE_TIME{
	/** Current Day. */
	uint8_t 	Day;
	/** Current Month. */
	uint8_t 	Month;
	/** Current Year. */
	uint16_t 	Year;
	/** Current Hour. */
	uint8_t		Hour;
	/** Current Minute. */
	uint8_t		Minute;
	/** Current Second. */
	uint8_t		Second;
};
/**@}*/

/**
 * @brief Singleton RTC Module.
 */
class __W_RTC : public __iW_Module, public iSingleton{
private:
	/**
	 * @brief Handle to the RTC library.
	 */
	PCF8563 RTC;
	
	/**
	 * @brief virtual implementation of the iW_Module function.
	 * Will report an error if not initialized.
	 * @return true RTC has been initialized.
	 * @return false RTC has not been initialized.
	 */
	virtual bool checkInitialized();

	// remove access to the constructor of __W_RTC.
	__W_RTC(){}

public:
	/**
	 * @brief Get the singleton instance of the class
	 * This function makes sure that only one instance is created and accessible during runtime.
	 * @return __W_RTC& the handle to the singleton instance
	 */
	static __W_RTC& getInstance(){
		static __W_RTC Instance;	// will only be destroyed on program exit.
		return Instance;
	}
public:
	// Assure that only one instance can exist by removing copy and assign functions.
	__W_RTC(__W_RTC const&)			= delete;	// delete copy constructor.
	void operator=(__W_RTC const&)	= delete;	// remove assignment operator.

	/**
	 * @brief Initializes the RTC object. Should only be called once.
	 * This function initializes the RTC object. It has a check build in to see if this function has already been called before. If so it will just return 0.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type init();
	
	/**
	 * @brief Returns the current date time.
	 * @return the current date time in a RTC_DATE_TIME struct.
	 */
	RTC_DATE_TIME read();

	/**
	 * @brief Returns a datetime in a string format.
	 * @return a datetime in a YYYY-MM-DD_hh.mm.ss format.
	 */
	String stringDateTime();

	/**
	 * @brief Returns the time in a string format.
	 * @return a time string in a hh.mm.ss format.
	 */
	String stringTime();
};