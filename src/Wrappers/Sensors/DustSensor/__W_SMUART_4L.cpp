#include <arduino.h>
#include "__W_SMUART_4L.h"
#include "../../../Logger/Logger.h"

#define RXD2 16
#define TXD2 17

bool __W_SM_UART_4L::checkInitialized(){
    if(Initialized){
        return 0;
    }
    else{
        Logger::getInstance().println("SM_UART_4L is not initialized!", LogLevel::Error);
        return 1;
    }
}

ERR_Type __W_SM_UART_4L::init() {
	if(Initialized){return SUCCESS;}	// return if already initialized;
	Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
	if(!aqi.begin_UART(&Serial2)){
		Logger::getInstance().println("Could not find SM_UART_4L sensor.", LogLevel::Error);
		return NO_LDS_SENSOR;
	}
	Logger::getInstance().println("SM_UART_4L found and initialized.", LogLevel::Info);
	Initialized = true;
	return SUCCESS;
}

ERR_Type __W_SM_UART_4L::read(PM25_AQI_Data& data){
	if(checkInitialized()){return NOT_INITIALIZED;} // don't act on to the hardware if not properly intialized;
	if(!aqi.read(&data)){
		Logger::getInstance().println("Could not read from SM_UART_4L", LogLevel::Warning);
		return READ_FAIL;
	}
	return SUCCESS;
}