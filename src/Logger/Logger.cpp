#include "Logger.h"
#include "../Wrappers/RTC/__W_RTC.h"
#include "../Defines/Defines.h"
#include <Arduino.h>

bool Logger::checkInitialized(){
	if(Initialized){
        return 0;
    }
    else{
        Logger::println("Logger is not initialized!", LogLevel::Warning);
        return 1;
    }
}

// initialize the logger & SD card.
ERR_Type Logger::init(){
	if(Initialized){return ALREADY_INITIALIZED;}
	Serial.begin(BAUDRATE);
	if(__W_RTC::getInstance().init()){
		Serial.println("RTC begin error!");
	}
	RTC_DATE_TIME RTC_DT = __W_RTC::getInstance().read();
	curr_day = RTC_DT.Day;
	
	ERR_Type ET = __W_SD::getInstance().init();
	if(ET){
		Logger::println("SD card failed to initialize!", LogLevel::Error);
		Logger::println("SD Error: " + String(ET), LogLevel::Error);
		return SD_NOT_INIT;
	}

	__W_SD::getInstance().createDir(("/" + String(RTC_DT.Year) ).c_str());
	__W_SD::getInstance().createDir(("/" + String(RTC_DT.Year)+"/"+String(RTC_DT.Month)).c_str());
	__W_SD::getInstance().createDir(("/" + String(RTC_DT.Year)+"/"+String(RTC_DT.Month)+"/"+String(RTC_DT.Day)).c_str());
	filepath = ("/"+String(RTC_DT.Year)+"/"+String(RTC_DT.Month)+"/"+String(RTC_DT.Day)+"/"+
					String(RTC_DT.Hour)+"."+String(RTC_DT.Minute)+"."+String(RTC_DT.Second) + ".txt");
	__W_SD::getInstance().writeFile(filepath.c_str(), String(
	"---------------------------------------------------------------------------------------------------\n"
	"[Start]   Start of Logfile\n"
	"[Date]    " + __W_RTC::getInstance().stringDateTime() + "\n"
	"[Version] " + SenseBox_VERSION + "\n"
	"---------------------------------------------------------------------------------------------------").c_str());
	
	Initialized = true;
	return SUCCESS;
}

// print the loglevel type to serial
void Logger::SER_print_LL_type(LogLevel LL){
	if(SER_line_ended){ // make sure to only add this at the beginning of a line
		Serial.print("[" + __W_RTC::getInstance().stringTime() + "]");
		switch (LL){
			case LogLevel::Error:
				Serial.print("[Err ]: ");
				break;
			case LogLevel::Warning:
				Serial.print("[Warn]: ");
				break;
			case LogLevel::Info:
				Serial.print("[Info]: ");
				break;
			case LogLevel::DataDump:
				Serial.println("[Dump]: Start ----------\n");
				break;
			default:
				break;
		}
		SER_line_ended = false;
	}
}

// print the loglevel type to SD
void Logger::SD_print_LL_type(LogLevel LL){
	if(SD_line_ended){ // make sure to only add this at the beginning of a line
		__W_SD::getInstance().appendFile(filepath.c_str(), ("[" + __W_RTC::getInstance().stringTime() + "] ").c_str());
		switch (LL){
			case LogLevel::Error:
				__W_SD::getInstance().appendFile(filepath.c_str(), "[Err ]: ");
				break;
			case LogLevel::Warning:
				__W_SD::getInstance().appendFile(filepath.c_str(), "[Warn]: ");
				break;
			case LogLevel::Info:
				__W_SD::getInstance().appendFile(filepath.c_str(), "[Info]: ");
				break;
			case LogLevel::DataDump:
				__W_SD::getInstance().appendFile(filepath.c_str(), "[Dump]: Start ----------\n");
				break;
			default:
				break;
		}
		SD_line_ended = false;
	}
}

void Logger::print(String s, LogLevel LL, LogType LT){
	PREV_LL = LL;

	// write to serial
	if(((uint8_t)(PREV_LL) <= DEBUGLEVEL) && ((uint8_t)(LT) & (uint8_t)(LogType::Serial))){
		SER_print_LL_type(LL);
		Serial.print(s);
	}
	// write to sd
	if(((uint8_t)(PREV_LL) <= LOGLEVEL) && ((uint8_t)(LT) & (uint8_t)(LogType::SD)) && Initialized){
		SD_print_LL_type(LL);
		__W_SD::getInstance().appendFile(filepath.c_str(), s.c_str());
	}
}

void Logger::println(String s, LogLevel LL, LogType LT){
	PREV_LL = LL;
	
	// write to serial
	if(((uint8_t)(PREV_LL) <= DEBUGLEVEL) && ((uint8_t)(LT) & (uint8_t)(LogType::Serial))){
		SER_print_LL_type(LL);
		Serial.println(s);
		SER_line_ended = true; // signal that for the next print statement a debug indicator should be added at the start.
	}
	// write to sd
	if(((uint8_t)(PREV_LL) <= LOGLEVEL) && ((uint8_t)(LT) & (uint8_t)(LogType::SD)) && Initialized){
		SD_print_LL_type(LL);
		__W_SD::getInstance().appendFile(filepath.c_str(), (s + String("\n")).c_str());
		SD_line_ended = true; // signal that for the next print statement a debug indicator should be added at the start.
	}
}

// writes a character to the file & serial
void Logger::write(char s, LogLevel LL, LogType LT){
	PREV_LL = LL;
	// write to serial
	if(((uint8_t)(PREV_LL) <= DEBUGLEVEL) && ((uint8_t)(LT) & (uint8_t)(LogType::Serial))){
		SER_print_LL_type(LL);
		Serial.write(s);
	}
	// write to sd
	//if(((uint8_t)(PREV_LL) <= LOGLEVEL) && ((uint8_t)(LT) & (uint8_t)(LogType::SD)) && Initialized){
	//	SD_print_LL_type(LL);
	//	//! NOTE: breekt dingen waarschijnlijk :)
	// 	//! NOTE: het breekt inderdaad dingen :)
	//	__W_SD::getInstance().appendFile(filepath.c_str(), &s);
	//}
}

// writes a new line & tab to the file
void Logger::breakLine(LogType LT){
	if(((uint8_t)(PREV_LL) <= DEBUGLEVEL) && ((uint8_t)(LT) & (uint8_t)(LogType::Serial))){
		Serial.print("\n\t\t");
	}
	if(((uint8_t)(PREV_LL) <= LOGLEVEL) && ((uint8_t)(LT) & (uint8_t)(LogType::SD)) && Initialized){
		__W_SD::getInstance().appendFile(filepath.c_str(), "\n\t\t");
	}
}

// signal that the datadump has ended
void Logger::dataDumpEnd(LogType LT){
	Logger::println("\n[Dump]: ---------- End", LogLevel::DataDump, LT);
}