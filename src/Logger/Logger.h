/**
 * @file Logger.h
 * @author Imre Korf
 *  Logger class that handles all the types of logging.
 * @version 0.1
 * @date 2021-11-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <Arduino.h>
#include "../Wrappers/SD/__W_SD.h"
#include "../Wrappers/Singleton/Singleton.h"
#include "../Defines/Defines.h"

/**
 * @defgroup ENUM Global Enumerations
 * @{
 */
/**
 *  Enum values that indicate the log level of the message.
 * @see LogType
 * @see Logger::print()
 * @see Logger::println()
 * @see Logger::write()
 */
enum class LogLevel {
	/** (0) Error level, This is used for when an error occures that can break the rest of the system. */
	Error,
	/** (1) Warning level, This is used for when an error occures that does not have a signficiant impact on the system. */
	Warning,
	/** (2) Info level, This is used for logging info or debugging info to the SD card. */
	Info,
	/** (3) DataDump, This is a special type of logging used for big blocks of data. */
	DataDump,
	/** (4) SerialInfo, This is used for logging SD-print information messages. Specifically for appending and writing to a file. */
	SD_printInfo
};

/**
 *  Enum values that indicate to which logging device should be logged.
 * @see LogLevel
 * @see Logger::print()
 * @see Logger::println()
 * @see Logger::write()
 * @see Logger::breakLine()
 * @see Logger::dataDumpEnd()
 */
enum class LogType {
	/** (1) SD card, Flag to only log the passed info to the SD card. */
	SD 			= 0b01,
	/** (2) Serial, Flag to only log the passed info to the Serial. */
	Serial  	= 0b10,
	/** (3) Serial_SD, Flag to log to the passed info to both the Serial and SD. */
	Serial_SD 	= 0b11 
};
/**@}*/

// Logger is the master of Serial1 aswell as the SD card.

/**
 *  Logger singleton class.
 * Logger is implemented as a Singleton to prevent two simultaneous logger instances from accessing the serial or sd.
 */
class Logger : public __iW_Module, public iSingleton{
private:
	/**
	 *  Handle to the SD card singleton.
	 */
	__W_SD* sdhandle;

	/**
	 *  Tracks if the serial line has ended.
	 */
	bool SER_line_ended = false;
	/**
	 *  Tracks if the SD card line has ended.
	 */
	bool SD_line_ended  = false;

	/**
	 *  Tracks the current LogLevel.
	 */
	LogLevel PREV_LL;
	/**
	 *  Contains the filepath to the current log file on the SD card.
	 */
	String filepath = "";
	/**
	 *  Tracks the current day, used to check if the day has changed. Indicating a new log file should be made.
	 */
	uint8_t curr_day = 32;

	/**
	 *  Used to create the header of a log message on serial. Looks like: "[Time][LogLevel] message".
	 * This function is only called at the start of a new log message. 
	 * This is tracked by the SER_line_ended variable, which is set to true once the Logger::println()
	 * has been called.
	 * @see Logger::println()
	 * @param LL The loglevel.
	 * 
	 */
	void SER_print_LL_type(LogLevel LL);
	/**
	 *  Used to create the header of a log message on the SD. Looks like: "[Time][LogLevel] message".
	 * This function is only called at the start of a new log message. 
	 * This is tracked by the SD_line_ended variable, which is set to true once the Logger::println()
	 * has been called.
	 * @see Logger::println()
	 * @param LL The loglevel.
	 */
	void SD_print_LL_type(LogLevel LL);

	/**
	 *  virtual implementation of the iW_Module function.
	 * Will report an error if not initialized.
	 * @return true SD card has been initialized.
	 * @return false SD card has not been initialized.
	 */
	virtual bool checkInitialized();	

public:
	/**
	 *  Get the Instance of the Logger object.
	 * This function makes sure that only one instance is created and accessible during runtime.
	 * @return Logger& The singleton instance of the object.
	 */
	static Logger& getInstance(){
		static Logger Instance;	// will only be destroyed on program exit.
		return Instance;
	}
	
private:	
	// remove access to the constructor of logger.
	Logger(){}
public:
	// Assure that only one instance can exist by removing copy and assign functions.
	Logger(Logger const&)			= delete;	// delete copy constructor.
	void operator=(Logger const&)	= delete;	// remove assignment operator.

	/**
	 *  Initializes the Logger object. Should only be called once.
	 * This function initializes the Logger object. It has a check build in to see if this function has already been called before. If so it will just return 0.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type init();

	// print statements.
	// ----------

	/**
	 *  Prints the given value to the specified output with the given loglevel.
	 * To end the log entry call Logger::println().
	 * @tparam T the datatype of the passed value. Should be convertable to a string.
	 * @param value the value to be printed.
	 * @param LL the log level.
	 * @param LT the output to log to.
	 * @see Logger::println()
	 * @see Logger::breakLine()
	 */
	template<typename T>
	void print(T value, LogLevel LL, LogType LT = LogType::Serial_SD){
		print(String(value), LL, LT);
	}

	/**
	 *  Prints the given value to the specified output with the given loglevel and ends the line.
	 * Call this function when a log entry ends. If you need multiple prints in the same log entry use Logger::print().
	 * @tparam T the datatype of the passed value. Should be convertable to a string.
	 * @param value the value to be printed.
	 * @param LL the log level.
	 * @param LT the output to log to.
	 * @see Logger::print()
	 * @see Logger::breakLine()
	 */
	template<typename T>
	void println(T value, LogLevel LL, LogType LT = LogType::Serial_SD){
		println(String(value), LL, LT);
	}

	/**
	 *  Prints the given value to the specified output with the given loglevel.
	 * To end the log entry call Logger::println().
	 * @tparam T the datatype of the passed value. Should be convertable to a string.
	 * @param s the string to be printed.
	 * @param LL the log level.
	 * @param LT the output to log to.
	 * @see Logger::println()
	 * @see Logger::breakLine()
	 */
	void print(String s, LogLevel LL, LogType LT = LogType::Serial_SD);
	/**
	 *  Prints the given value to the specified output with the given loglevel and ends the line.
	 * Call this function when a log entry ends. If you need multiple prints in the same log entry use Logger::print().
	 * @tparam T the datatype of the passed value. Should be convertable to a string.
	 * @param s the value to be printed.
	 * @param LL the log level.
	 * @param LT the output to log to.
	 * @see Logger::print()
	 * @see Logger::breakLine()
	 */
	void println(String s, LogLevel LL, LogType LT = LogType::Serial_SD);
	/**
	 *  writes the given character to the specified output with the given loglevel. Use this with loglevel DataDump and Logger::dataDumpEnd().
	 * To end the log entry call Logger::println().
	 * @tparam T the datatype of the passed value. Should be convertable to a string.
	 * @param s the value to be printed.
	 * @param LL the log level.
	 * @param LT the output to log to.
	 * @see Logger::print()
	 * @see Logger::println()
	 * @see Logger::breakLine()
	 * @see Logger::dataDumpEnd()
	 */
	void write(char s, LogLevel LL, LogType LT = LogType::Serial_SD);
	/**
	 *  adds a linebreak to a log entry without ending it.
	 * @param LT The Loglevel.
	 */
	void breakLine(LogType LT = LogType::Serial_SD);
	/**
	 *  End the started datadump entry.
	 * @param LT The Loglevel.
	 */
	void dataDumpEnd(LogType LT = LogType::Serial_SD);
};