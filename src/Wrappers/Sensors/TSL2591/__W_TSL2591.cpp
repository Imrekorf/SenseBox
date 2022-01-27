#include "__W_TSL2591.h"
#include <Arduino.h>
#include "../../../Logger/Logger.h"

bool __W_TSL2591::checkInitialized(){
    if(Initialized){
        return 0;
    }
    else{
        Logger::getInstance().println("TSL2591 is not initialized!", LogLevel::Error);
        return 1;
    }
}

/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2591.
*/
/**************************************************************************/
ERR_Type __W_TSL2591::init(){
    if(Initialized){return ALREADY_INITIALIZED;}	// return if already initialized.
    if (tsl.begin()) 
    {
		Logger::getInstance().println("Found a TSL2591 sensor", LogLevel::Info);
	} 
	else 
	{
		Logger::getInstance().println("No TSL2591 sensor found ... check your wiring?", LogLevel::Error);
		return TSL_BEGIN_ERR; // error code
    }

    // You can change the gain on the fly, to adapt to brighter/dimmer light situations
    //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
    tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
    //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain

    // Changing the integration time gives you a longer time over which to sense light
    // longer timelines are slower, but are good in very low light situtations!
    //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
    tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

    /* Display the gain and integration time for reference sake */  
    Logger::getInstance().print("TSL2591 bootup\n", LogLevel::Info);
    Logger::getInstance().print("------------------------------------\n", LogLevel::Info);
    Logger::getInstance().print("Gain:         ", LogLevel::Info);
    tsl2591Gain_t gain = tsl.getGain();
    switch(gain)
    {
    	case TSL2591_GAIN_LOW:
			Logger::getInstance().print("1x (Low)\n", LogLevel::Info);
			break;
      	case TSL2591_GAIN_MED:
			Logger::getInstance().print("25x (Medium)\n", LogLevel::Info);
			break;
      	case TSL2591_GAIN_HIGH:
			Logger::getInstance().print("428x (High)\n", LogLevel::Info);
			break;
      	case TSL2591_GAIN_MAX:
			Logger::getInstance().print("9876x (Max)\n", LogLevel::Info);
			break;
    }
    Logger::getInstance().print  ("Timing:       ", LogLevel::Info);
    Logger::getInstance().print((tsl.getTiming() + 1) * 100, LogLevel::Info); 
    Logger::getInstance().print(" ms\n", LogLevel::Info);
    Logger::getInstance().println("------------------------------------\n", LogLevel::Info);

    Initialized = true;
    return SUCCESS;
}


/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void __W_TSL2591::displaySensorDetails(void)
{
    if(checkInitialized()){return;} // don't act on to the hardware if not properly intialized;
    sensor_t sensor;
    tsl.getSensor(&sensor);
    Logger::getInstance().print("\nTSL2591 Sensor Info: ", LogLevel::Info);
    Logger::getInstance().print("\n------------------------------------", LogLevel::Info);
    Logger::getInstance().print("\nSensor:       ", LogLevel::Info); Logger::getInstance().print(sensor.name, LogLevel::Info);
    Logger::getInstance().print("\nDriver Ver:   ", LogLevel::Info); Logger::getInstance().print(sensor.version, LogLevel::Info);
    Logger::getInstance().print("\nUnique ID:    ", LogLevel::Info); Logger::getInstance().print(sensor.sensor_id, LogLevel::Info);
    Logger::getInstance().print("\nMax Value:    ", LogLevel::Info); Logger::getInstance().print(sensor.max_value, LogLevel::Info); Logger::getInstance().println(" lux", LogLevel::Info);
    Logger::getInstance().print("\nMin Value:    ", LogLevel::Info); Logger::getInstance().print(sensor.min_value, LogLevel::Info); Logger::getInstance().println(" lux", LogLevel::Info);
    Logger::getInstance().print("\nResolution:   ", LogLevel::Info); Logger::getInstance().print(sensor.resolution, LogLevel::Info); Logger::getInstance().println(" lux", LogLevel::Info);  
    Logger::getInstance().println("\n------------------------------------", LogLevel::Info);
    delay(500);
}

uint16_t __W_TSL2591::getLuminosity(uint8_t spectrum){
    if(checkInitialized()){return 0;} // don't act on to the hardware if not properly intialized;
    return tsl.getLuminosity(spectrum);
}

TSL2591_DATA __W_TSL2591::getFullLuminosity(){
    TSL2591_DATA LB;
    if(checkInitialized()){return LB;} // don't act on to the hardware if not properly intialized;
    uint32_t lum = tsl.getFullLuminosity();
    LB.ir = lum >> 16; // get last 16 bits
    LB.full = lum & 0xFFFF; // get first 16 bits
    LB.visible = LB.full-LB.ir;
    return LB;
}

float __W_TSL2591::getLux(uint16_t full, uint16_t ir){
    if(checkInitialized()){return 0.0;} // don't act on to the hardware if not properly intialized;
    return tsl.calculateLux(full, ir);
}