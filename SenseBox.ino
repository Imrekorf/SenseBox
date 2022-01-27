/**
 * @file SenseBox.ino
 * @author Imre Korf
 * @brief Main sketch file for the IoT minor
 * @version 0.1
 * @date 2021-11-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// TODO: what to do when SD card is unplugged? implement errors
// TODO: weird bug with the hour value increasing with 40 when the second value is above 39.
// TODO: implement only storing 10 log files
// TODO: fix bug with the rtc giving wrong day values
// TODO: fix ambimate reading 0's the first time. Also fix ambimate event reading.
// TODO: implement all the other things
// TODO: fix AS7262 color sensor updating values :)


#include "src/MQTT/MQTT.h"
//#include "src/Sbox/Sbox.h"
#include "src/Logger/Logger.h"

//SBox Sbox;
MQTTClient M_Client;


void setup(){
  pinMode(18, OUTPUT);
	if(Logger::getInstance().init()){
    while(1);
	}
	//Sbox.init();  
	// MQTT INIT
	M_Client.init("/MQTTSettings.dat");  
}

//ColorSpectrum CS;
//PM25_AQI_Data DUST;

void loop(){
/*
  //
  //digitalWrite(18, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);                       // wait for a second
  //Logger::getInstance().println("OFF", LogLevel::Info);
  //digitalWrite(18, LOW);    // turn the LED off by making the voltage LOW
  //delay(1000);                       // wait for a second
*/
	AmbimateData A_DAT = Sbox.getAmbimateData();

	Sbox.getColorSpectrum(CS);	
	//Sbox.getLDSData(DUST);

	int audio = Sbox.getMax4466();
	float O2val = Sbox.getO2();
	SCD30_DATA SCD30_D = Sbox.getSCD30Data();
	TSL2591_DATA TSL_DAT = Sbox.getTSL2591Data();
	RTC_DATE_TIME datetime = Sbox.getTime();
	
  Logger::getInstance().println("ambimate data", LogLevel::Info);
  Logger::getInstance().print("Temperatuur: ", LogLevel::Info); Logger::getInstance().print(A_DAT.temperatureC, LogLevel::Info); Logger::getInstance().println(" C", LogLevel::Info); 
  Logger::getInstance().print("Humidity: ", LogLevel::Info); Logger::getInstance().print(A_DAT.Humidity, LogLevel::Info); Logger::getInstance().println(" RH", LogLevel::Info); 
  Logger::getInstance().print("BatVolts: ", LogLevel::Info); Logger::getInstance().print(A_DAT.batVolts, LogLevel::Info); Logger::getInstance().println(" V", LogLevel::Info); 
  Logger::getInstance().print("Audio: ", LogLevel::Info); Logger::getInstance().print(A_DAT.audio, LogLevel::Info); Logger::getInstance().println("", LogLevel::Info); 
  Logger::getInstance().print("eco2: ", LogLevel::Info); Logger::getInstance().print(A_DAT.eco2_ppm, LogLevel::Info); Logger::getInstance().println(" ppm", LogLevel::Info); 
  Logger::getInstance().print("voc: ", LogLevel::Info); Logger::getInstance().print(A_DAT.voc_ppm, LogLevel::Info); Logger::getInstance().println(" ppm", LogLevel::Info); 
  Logger::getInstance().print("motion event: ", LogLevel::Info); Logger::getInstance().print(A_DAT.MOT_EVENT, LogLevel::Info); Logger::getInstance().println("", LogLevel::Info); 
  Logger::getInstance().print("Audio event: ", LogLevel::Info); Logger::getInstance().print(A_DAT.AUD_EVENT, LogLevel::Info); Logger::getInstance().println("", LogLevel::Info); 
  Logger::getInstance().print("PIR event: ", LogLevel::Info); Logger::getInstance().print(A_DAT.PIR_EVENT, LogLevel::Info); Logger::getInstance().println("\n", LogLevel::Info); 

  Logger::getInstance().println("AS7262 data", LogLevel::Info);
  Logger::getInstance().print("Violet: ", LogLevel::Info); Logger::getInstance().print(CS.Violet, LogLevel::Info); Logger::getInstance().println("", LogLevel::Info); 
  Logger::getInstance().print("Blue: ", LogLevel::Info); Logger::getInstance().print(CS.Blue, LogLevel::Info); Logger::getInstance().println("", LogLevel::Info); 
  Logger::getInstance().print("Green: ", LogLevel::Info); Logger::getInstance().print(CS.Green, LogLevel::Info); Logger::getInstance().println("", LogLevel::Info); 
  Logger::getInstance().print("Yellow: ", LogLevel::Info); Logger::getInstance().print(CS.Yellow, LogLevel::Info); Logger::getInstance().println("", LogLevel::Info); 
  Logger::getInstance().print("Orange: ", LogLevel::Info); Logger::getInstance().print(CS.Orange, LogLevel::Info); Logger::getInstance().println("", LogLevel::Info); 
  Logger::getInstance().print("Red: ", LogLevel::Info); Logger::getInstance().print(CS.Red, LogLevel::Info); Logger::getInstance().println("\n", LogLevel::Info); 

  Logger::getInstance().println("TSL2591 data", LogLevel::Info);
  Logger::getInstance().print("Visible: ", LogLevel::Info); Logger::getInstance().print(TSL_DAT.visible, LogLevel::Info); Logger::getInstance().println(" lux", LogLevel::Info); 
  Logger::getInstance().print("IR: ", LogLevel::Info); Logger::getInstance().print(TSL_DAT.ir, LogLevel::Info); Logger::getInstance().println(" lux", LogLevel::Info); 
  Logger::getInstance().print("Full: ", LogLevel::Info); Logger::getInstance().print(TSL_DAT.full, LogLevel::Info); Logger::getInstance().println(" lux\n", LogLevel::Info); 
  
  Logger::getInstance().println("SCD30 data", LogLevel::Info);
  Logger::getInstance().print("CO2: ", LogLevel::Info); Logger::getInstance().print(SCD30_D.CO2, LogLevel::Info); Logger::getInstance().println("ppm", LogLevel::Info); 
  Logger::getInstance().print("Temperature: ", LogLevel::Info); Logger::getInstance().print(SCD30_D.Temperature, LogLevel::Info); Logger::getInstance().println("C", LogLevel::Info); 
  Logger::getInstance().print("Humidity: ", LogLevel::Info); Logger::getInstance().print(SCD30_D.Humidity, LogLevel::Info); Logger::getInstance().println("RH\n", LogLevel::Info); 
  
  Logger::getInstance().println("MIX8410 data", LogLevel::Info);
  Logger::getInstance().print("O2: ", LogLevel::Info); Logger::getInstance().print(O2val, LogLevel::Info); Logger::getInstance().println("\n", LogLevel::Info); 
  
  Logger::getInstance().println("MAX4466", LogLevel::Info);
  Logger::getInstance().print("Audio: ", LogLevel::Info); Logger::getInstance().print(audio, LogLevel::Info); Logger::getInstance().println("\n", LogLevel::Info); 
  
  
  
	// MQTT UPDATE
	M_Client.sendData(attribute_names[CONN],  "Connected");
	M_Client.sendData(attribute_names[MAX4466_Audio], String(audio).c_str());
	M_Client.sendData(attribute_names[MIX8410_O2], String(O2val).c_str());

	M_Client.sendData(attribute_names[SCD30_CO2],String(SCD30_D.CO2).c_str());
	M_Client.sendData(attribute_names[SCD30_hum],   String(SCD30_D.Humidity).c_str());
	M_Client.sendData(attribute_names[SCD30_temp],  String(SCD30_D.Temperature).c_str());

	M_Client.sendData(attribute_names[ambimate_Voc],String(A_DAT.voc_ppm).c_str());
  M_Client.sendData(attribute_names[ambimate_hum],String(A_DAT.Humidity).c_str());
  M_Client.sendData(attribute_names[ambimate_temp],String(A_DAT.temperatureC).c_str());
  M_Client.sendData(attribute_names[ambimate_Eco2],String(A_DAT.eco2_ppm).c_str());

  String JsonColor = "{\"Violet\":" + String(CS.Violet) + 
                    ", \"Blue\":" + String(CS.Blue) + 
                    ", \"Green\":" + String(CS.Green) + 
                    ", \"Yellow\":" + String(CS.Yellow) + 
                    ", \"Orange\":" + String(CS.Orange) + 
                    ", \"Red\":" + String(CS.Red) + "}";
  M_Client.sendData(attribute_names[AS7262_Color],JsonColor.c_str());
  String JsonSpectrum = "{\"Visible\":" + String(TSL_DAT.visible) + 
                        ", \"IR\":" + String(TSL_DAT.ir) + 
                        ", \"Full\":" + String(TSL_DAT.full) + "}";
  M_Client.sendData(attribute_names[TSL2591_Spectrum],JsonSpectrum.c_str());

	//M_Client.sendData(attribute_names[PM10],  String(DUST.particles_10um).c_str());
	//M_Client.sendData(attribute_names[PM25],  String(DUST.particles_25um).c_str());
	//M_Client.sendData(attribute_names[PM100], String(DUST.particles_100um).c_str());

	M_Client.loopClient();
}
