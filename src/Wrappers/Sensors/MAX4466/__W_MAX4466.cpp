#include <Arduino.h>
#include "__W_MAX4466.h"
#include "../../../Logger/Logger.h"

#include <Arduino.h>
#include <esp_adc_cal.h>

// AUD_IN pin is GPIO36, ADC1 CH0

bool __W_MAX4466::checkInitialized(){
	if(Initialized){
        return 0;
    }
    else{
        Logger::getInstance().println("MAX4466 is not initialized!", LogLevel::Error);
        return 1;
    }
}

ERR_Type __W_MAX4466::init(){
	if(Initialized){return ALREADY_INITIALIZED;}	// return if already initialized;
	
	// range 0-4096
	adc1_config_width(ADC_WIDTH_BIT_12);

	// full voltage range
	adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);

	Initialized = true;

	return SUCCESS;
}

int __W_MAX4466::read(){
	// read a sample from the ADC
	return adc1_get_raw(ADC1_CHANNEL_0);
}