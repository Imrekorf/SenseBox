/**
 * @file Defines.h
 * @author Imre Korf
 * @brief Defines file used for global precompile settings
 * @version 0.1
 * @date 2021-11-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

/**
 * @defgroup DEFINES Global defines
 * This list contains values of different definitions used throughout the project. 
 * @{
 */

/**
 * @brief Version of the Sensebox software
 * A change in the first  version indicator is a major version change which might not be compatible with older major versions
 * A change in the second version indicator is a minor version change which should be compatible with other of the same major versions
 * A change in the third  version indicator is a tweak version change, this will include mostly small bug fixes  
 */
#define SenseBox_VERSION "1.0.1"

/**
 * Controls the baudrate of the serial port connected to the USB port.
 */
#define BAUDRATE 115200

/** 
 * @brief Used by the source code to indicate what to debug to serial monitor
 * Value | Description
 * :-------:|:-----------------------------:
 *  0 | Print nothing to serial
 *  1 | Print errors to serial
 *  2 | Print errors and warnings to serial
 *  3 | Print errors, warnings & info to serial
 * 	4 | Print errors, warnings, info & data-dumps to serial
 *  5 | Print errors, warnings, info, data-dumps & SD-info to Serial
 */
#define DEBUGLEVEL 3
/**
 * @brief Used by the source code to indicate what to log to the SD card
 * 
 * Value | Description
 * :-------:|:-----------------------------:
 *  0 | Log nothing
 *  1 | Logs only errors
 *  2 | Logs errors & warnings
 *  3 | Logs errors, warnings & info
 *  4 | Logs errors, warnings, info & data-dumps
 */
#define LOGLEVEL 3

/**
 * @brief The amount of retries the ESP32 will try to connect to the provided WiFi network before timing out.
 */
#define WIFI_TIMEOUT 50
/**
 * @brief The amount of retries the ESP32 will try to connect to the MQTT Broker before timing out.
 */
#define MQTT_CONN_TIMEOUT 20

/** @} */


/**
 * @addtogroup ENUM
 * This list details the global enumerations used in the project.
 * @{
 */

/**  List of function return errors
 */
enum ERR_Type {
	//==== Successful exits ====//

	/** SUCCESS, No error occured. */
	SUCCESS = 0,
	/** READ_SUCCESS, A read operation was succesfull. */
	READ_SUCCESS = 0,
	/** ALREADY_INITIALIZED, Module's init function was called multiple times. */
	ALREADY_INITIALIZED = 0,

	//==== Errors ====//

	/** ERROR, generic error */
	ERROR = 1,
	/** READ_ERROR, A generic read error */
	READ_ERROR = 1,
	/** READ_FAIL, A read operation failed. */
	READ_FAIL = 1,
	/** NOT_INITIALIZED, Indicates that the module has not been initialized correctly. */
	NOT_INITIALIZED,

	// Logger Errors
	/** SD_NOT_INIT, indicates that the SD has not been properly initialized. */
	SD_NOT_INIT,

	// SBOX Errors
	/** MOD_INIT_ERR, indicates that a module was not succesfully initialized. */
	MOD_INIT_ERR,

	// RTC Errors
	/** RTC_BEGIN_ERR, indicates that the .begin() method has failed. */
	RTC_BEGIN_ERR,

	// SD Errors
	/** SD_BEGIN_ERR, indicates that the .begin() method has failed. */
	SD_BEGIN_ERR,
	/** NO_SD_CARD, indicates that no SD card was inserted. */
	NO_SD_CARD,
	/** SD_CARDTYPE_NONE, indicates that the inserted SD card type is not supported. */
	SD_CARDTYPE_NONE,
	/** SD_DIR_OPEN_FAIL, failed to open given directory. */
	SD_DIR_OPEN_FAIL,
	/** SD_NOT_A_DIR, given directory does not exist */
	SD_NOT_A_DIR,
	/** SD_MKDIR_FAIL, failed to create given directory */
	SD_MKDIR_FAIL,
	/** SD_RMDIR_FAIL, failed to remove given directory */
	SD_RMDIR_FAIL,
	/** SD_FILE_OPEN_FAIL, failed to open given file */
	SD_FILE_OPEN_FAIL,
	/** SD_WRITE_FAIL, failed to write message to file */
	SD_WRITE_FAIL,
	/** SD_APP_FAIL, failed to append message to file */
	SD_APP_FAIL,
	/** SD_RENAME_FAIL, failed to rename file */
	SD_RENAME_FAIL,
	/** SD_RM_FAIL, failed to remove file */
	SD_RM_FAIL,

	// Wifi & MQTT Errors
	/** WIFI_CONN_FAIL, failed to connect to wifi network */
	WIFI_CONN_FAIL,
	/** MQTT_CONN_FAIL, failed to connect to MQTT broker */
	MQTT_CONN_FAIL,
	/** WIFI_SETTINGS_EMPTY, provided settings file path is either invalid or file is empty */
	WIFI_SETTINGS_EMPTY,
	/** WIFI_SETTINGS_NOT_COMPLETE, provided settings file does not contain all settings */
	WIFI_SETTINGS_NOT_COMPLETE,
	/** WIFI_SETTINGS_NOT_COMPLETE, provided setting was over 50 characters */
	WIFI_SETTINGS_STRING_OVERLOAD,

	// Ambimate Errors
	/** AMBI_I2C_INIT_ERR, Aindicates that an I2C error occured in the init function. */
	AMBI_I2C_INIT_ERR,
	
	// AS7226X Errors
	/** AS726X_BEGIN_ERR, indicates that the .begin() method has failed. */
	AS726X_BEGIN_ERR,
	/** AS726X_DATA_NOT_READY, indicates that a ready was attempted but data was not yet ready */
	AS726X_DATA_NOT_READY,

	// LDS Errors
	/** NO_LDS_SENSOR, indicates that no LDS sensor was found. */
	NO_LDS_SENSOR,

	// SCD30 Errors
	/** SCD30_BEGIN_ERR, indicates that the .begin() method has failed. */
	SCD30_BEGIN_ERR,

	// TSL2591 Errors
	/** TSL_BEGIN_ERR, indicates that the .begin() method has failed. */
	TSL_BEGIN_ERR,

};
/**@}*/