#include "Sbox.h"
#include "../Logger/Logger.h"

#include <Wire.h>

ERR_Type SBox::init(){
	// start Wire
	Wire.begin();

	// get a handle to the singletons
	Ambimate = &__W_Ambimate::getInstance();
	AS7262 	 = &__W_AS726X::getInstance();
	LDS 	 = &__W_SM_UART_4L::getInstance();
	MAX4466  = &__W_MAX4466::getInstance();
	MIX8410  = &__W_MIX8410::getInstance();
	SCD30 	 = &__W_SCD30::getInstance();
	TSL2591  = &__W_TSL2591::getInstance();
	RTC		 = &__W_RTC::getInstance();

	// store all the handles as a base module for easy initialisation
	__iW_Module* Modules[] = {Ambimate, AS7262, LDS, MAX4466, MIX8410, SCD30, TSL2591};

	for(int i = 0; i < 7; i++){
		// initialize all the hardware modules.
		if(Modules[i]->init()){
			Logger::getInstance().println("Module [" + String(i) + "] was not properly intialized. Exiting...", LogLevel::Error);
			//return MOD_INIT_ERR;
		}
	}

	return SUCCESS;
}

RTC_DATE_TIME SBox::getTime(){
	return RTC->read();
}

AmbimateData SBox::getAmbimateData(){
	return Ambimate->read();
}

ERR_Type SBox::getColorSpectrum(ColorSpectrum& CS){
	if(AS7262->checkDataReady()){
		AS7262->getMeasurements(&CS);
		return SUCCESS;
	}
	return AS726X_DATA_NOT_READY;
}

ERR_Type SBox::getLDSData(PM25_AQI_Data& Buffer){
	return LDS->read(Buffer);
}

int SBox::getMax4466(){
	return MAX4466->read();
}

float SBox::getO2(){
	return MIX8410->readConcentration();
}

SCD30_DATA SBox::getSCD30Data(){
	return SCD30->read();
}

TSL2591_DATA SBox::getTSL2591Data(){
	return TSL2591->getFullLuminosity();
}