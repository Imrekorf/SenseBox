#include "__W_RTC.h"

#include <Wire.h>
#include <Arduino.h>
#include "../../Logger/Logger.h"

bool __W_RTC::checkInitialized(){
    if(Initialized){
        return 0;
    }
    else{
        Logger::getInstance().println("RTC is not initialized!", LogLevel::Error);
        return 1;
    }
}


ERR_Type __W_RTC::init() {
	if(Initialized){return ALREADY_INITIALIZED;}	// return if already initialized;
	if(!RTC.begin()){
		return RTC_BEGIN_ERR;
	}
	//if(!RTC.isRunning()){
		// set the date time based on these constant which are set on compile time
		RTC.setDateTime(__DATE__, __TIME__);
		// start the clock
		RTC.startClock();
		Initialized = true; // finish initialisation to prevent errors.

		
		Logger::getInstance().println("Setting up RTC!", LogLevel::Info, LogType::Serial);
		Logger::getInstance().println("New Time Set", LogLevel::Info, LogType::Serial);
		Logger::getInstance().print(__DATE__, LogLevel::Info, LogType::Serial);
		Logger::getInstance().print(" ", LogLevel::Info, LogType::Serial);
		Logger::getInstance().println(__TIME__, LogLevel::Info, LogType::Serial);
		
	//}
	/*
	Logger::getInstance().print("RTC starting time DD-MM-YYYY : hh:mm:ss : ", LogLevel::Info, LogType::Serial);
	Logger::getInstance().print(RTC.getDay(), LogLevel::Info, LogType::Serial); Logger::getInstance().print("-", LogLevel::Info, LogType::Serial);
	Logger::getInstance().print(RTC.getMonth(), LogLevel::Info, LogType::Serial); Logger::getInstance().print("-", LogLevel::Info, LogType::Serial);
	Logger::getInstance().print(RTC.getYear(), LogLevel::Info, LogType::Serial); Logger::getInstance().print(" : ", LogLevel::Info, LogType::Serial);
	Logger::getInstance().print(RTC.getHours(), LogLevel::Info, LogType::Serial); Logger::getInstance().print(":", LogLevel::Info, LogType::Serial);
	Logger::getInstance().print(RTC.getMinutes(), LogLevel::Info, LogType::Serial); Logger::getInstance().print(":", LogLevel::Info, LogType::Serial);
	Logger::getInstance().print(RTC.getSeconds(), LogLevel::Info, LogType::Serial); Logger::getInstance().println("", LogLevel::Info, LogType::Serial);
	*/
	return SUCCESS;
}

RTC_DATE_TIME __W_RTC::read(){
	RTC_DATE_TIME RTC_DT;
	if(checkInitialized()){return RTC_DT;} // don't act on to the RTC hardware if not properly intialized;
	
	RTC_DT.Day	 	= RTC.getDay();
	RTC_DT.Month	= RTC.getMonth();
	RTC_DT.Year 	= RTC.getYear();
	RTC_DT.Hour 	= RTC.getHours();
	RTC_DT.Minute 	= RTC.getMinutes();
	RTC_DT.Second 	= RTC.getSeconds();
	return RTC_DT;
}

String __W_RTC::stringDateTime(){
	if(checkInitialized()){return String("");} // don't act on to the RTC hardware if not properly intialized;
	return String(RTC.getYear()) + "-" + String(RTC.getMonth()) + "-" + String(RTC.getDay()) + " " + String(RTC.getHours()) + ":" + String(RTC.getMinutes()) + ":" + String(RTC.getSeconds());
}

String __W_RTC::stringTime(){
	if(checkInitialized()){return String("");} // don't act on to the RTC hardware if not properly intialized;
	return String(RTC.getHours()) + ":" + String(RTC.getMinutes()) + ":" + String(RTC.getSeconds());
}