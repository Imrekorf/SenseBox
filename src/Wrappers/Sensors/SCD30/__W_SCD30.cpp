#include "__W_SCD30.h"
#include "../../../Logger/Logger.h"

bool __W_SCD30::checkInitialized(){
    if(Initialized){
        return 0;
    }
    else{
        Logger::getInstance().println("SCD30 is not initialized!", LogLevel::Error);
        return 1;
    }
}

ERR_Type __W_SCD30::init(bool autoCalibrate){
	if(Initialized){return SUCCESS;}	// return if already initialized;
	this->autoCalibrate = autoCalibrate;
	return init();
}

ERR_Type __W_SCD30::init(){
	if(Initialized){return ALREADY_INITIALIZED;}	// return if already initialized;
	if(!airSensor.begin(autoCalibrate)){ 
		Logger::getInstance().println("SCD30 not detected. Please check wiring.", LogLevel::Error);
		return SCD30_BEGIN_ERR;
	}
	Logger::getInstance().println("SCD30 initialized.", LogLevel::Info);
	Initialized = true;
	return SUCCESS;
}

SCD30_DATA __W_SCD30::read(){
	SCD30_DATA buffer;
	if(checkInitialized()){return buffer;} // don't act on to the hardware if not properly intialized;
	buffer.CO2 = getCO2();
	buffer.Temperature = getTemperature();
	buffer.Humidity = getHumidity();
	return buffer;
}

bool __W_SCD30::dataAvailable(){
	if(checkInitialized()){return false;} // don't act on to the hardware if not properly intialized;
	return airSensor.dataAvailable();
}

uint16_t __W_SCD30::getCO2(){
	if(checkInitialized()){return 0;} // don't act on to the hardware if not properly intialized;
	return airSensor.getCO2();
}

float __W_SCD30::getTemperature(){
	if(checkInitialized()){return 0.0;} // don't act on to the hardware if not properly intialized;
	return airSensor.getTemperature();
}

float __W_SCD30::getHumidity(){
	if(checkInitialized()){return 0.0;} // don't act on to the hardware if not properly intialized;
	return airSensor.getHumidity();
}

// Options

void __W_SCD30::setMeasurementsInterval(uint16_t seconds){
	if(checkInitialized()){return;} // don't act on to the hardware if not properly intialized;
	airSensor.setMeasurementInterval(seconds);
	delay(200);
}
bool __W_SCD30::getMeasurementsInterval(uint16_t& seconds){
	if(checkInitialized()){return false;} // don't act on to the hardware if not properly intialized;
	return airSensor.getMeasurementInterval(&seconds);
}

void __W_SCD30::setAltitudeCompensation(uint16_t altitude){
	if(checkInitialized()){return;} // don't act on to the hardware if not properly intialized;
	airSensor.setAltitudeCompensation(altitude);
}
bool __W_SCD30::getAltitudeCompensation(uint16_t& altitude){
	if(checkInitialized()){return false;} // don't act on to the hardware if not properly intialized;
	return airSensor.getAltitudeCompensation(&altitude);
}

void __W_SCD30::setAmbientPressure(uint16_t offset){
	if(checkInitialized()){return;} // don't act on to the hardware if not properly intialized;
	airSensor.setAmbientPressure(offset);
}

void __W_SCD30::setTemperatureOffset(float tempOffset){
	if(checkInitialized()){return;} // don't act on to the hardware if not properly intialized;
	airSensor.setTemperatureOffset(tempOffset);
}

bool __W_SCD30::getTemperatureOffset(float& tempOffset){
	if(checkInitialized()){return false;} // don't act on to the hardware if not properly intialized;
	return airSensor.getTemperatureOffset((uint16_t*)&tempOffset);
}

bool  __W_SCD30::getAutoSelfCalibration(){
	if(checkInitialized()){return false;} // don't act on to the hardware if not properly intialized;
	return airSensor.getAutoSelfCalibration();
}

bool __W_SCD30::getForcedRecalibration(uint16_t& settingVal){
	if(checkInitialized()){return false;} // don't act on to the hardware if not properly intialized;
	return airSensor.getForcedRecalibration(&settingVal);
}

bool __W_SCD30::getFirmwareVersion(uint16_t& settingVal){
	if(checkInitialized()){return false;} // don't act on to the hardware if not properly intialized;
	return airSensor.getFirmwareVersion(&settingVal);
}