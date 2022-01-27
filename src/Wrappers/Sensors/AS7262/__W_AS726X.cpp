#include <Arduino.h>
#include "__W_AS726X.h"
#include "../../../Logger/Logger.h"

bool __W_AS726X::checkInitialized(){
    if(Initialized){
        return 0;
    }
    else{
        Logger::getInstance().println("AS7226X is not initialized!", LogLevel::Error);
        return 1;
    }
}

ERR_Type __W_AS726X::init(){
	if(Initialized){return ALREADY_INITIALIZED;}	// return if already initialized;
	if(!AS7262.begin()){
		Logger::getInstance().println("Could not connect to AS726X! Please check your wiring.", LogLevel::Error);
		return AS726X_BEGIN_ERR;	
	}
	Logger::getInstance().println("AS7262 initialized", LogLevel::Info);
	Initialized = true;
	return SUCCESS;
}

void __W_AS726X::setDrvLed(bool on){
	if(checkInitialized()){return;} // don't act on to the hardware if not properly intialized;
	if(on){	AS7262.drvOn();	}
	else{	AS7262.drvOff();}
}

void __W_AS726X::setDrvCurrent(uint8_t current){
	if(checkInitialized()){return;} // don't act on to the hardware if not properly intialized;
	AS7262.setDrvCurrent(current);
}

void __W_AS726X::indicateLED(bool on){
	if(checkInitialized()){return;} // don't act on to the hardware if not properly intialized;
	AS7262.indicateLED(on);
}

void __W_AS726X::setIndicateCurrent(uint8_t current){
	if(checkInitialized()){return;} // don't act on to the hardware if not properly intialized;
	AS7262.setIndicateCurrent(current);
}

void __W_AS726X::setGain(uint8_t gain){
	if(checkInitialized()){return;} // don't act on to the hardware if not properly intialized;
	AS7262.setGain(gain);
}

void __W_AS726X::setIntegrationTime(uint8_t time){
	if(checkInitialized()){return;} // don't act on to the hardware if not properly intialized;
	AS7262.setIntegrationTime(time);
}

void __W_AS726X::setConversionType(uint8_t type){
	if(checkInitialized()){return;} // don't act on to the hardware if not properly intialized;
	AS7262.setConversionType(type);
}

void __W_AS726X::startMeasurement(){
	if(checkInitialized()){return;} // don't act on to the hardware if not properly intialized;
	AS7262.startMeasurement();
}

bool __W_AS726X::checkDataReady(){
	if(checkInitialized()){return false;} // don't act on to the hardware if not properly intialized;
	return AS7262.dataReady();
}

void __W_AS726X::getMeasurements(ColorSpectrum* CS){
	if(checkInitialized()){return;} // don't act on to the hardware if not properly intialized;
	CS->Violet = AS7262.readCalibratedViolet();
	CS->Blue = AS7262.readCalibratedBlue();
	CS->Green = AS7262.readCalibratedGreen();
	CS->Yellow = AS7262.readCalibratedYellow();
	CS->Orange = AS7262.readCalibratedOrange();
	CS->Red = AS7262.readCalibratedRed();
}

uint8_t __W_AS726X::getTemperature(){
	if(checkInitialized()){return 0;} // don't act on to the hardware if not properly intialized;
	return AS7262.readTemperature();
}