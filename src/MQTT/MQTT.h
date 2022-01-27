/**
 * @file MQTT.h
 * @author Imre Korf
 * @brief MQTT header for managing MQTT and WiFi connections.
 * @version 0.1
 * @date 2022-01-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once 

#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "../Defines/Defines.h"

/**
 * @addtogroup ENUM
 * @{
 */

/**
 * @brief Attribute array indices for the attribute_names array.
 */
enum attributes {
    /** Connection, used to signify if the box is connected to the IoT platform.*/
    CONN,
    /** Ambimate temperature, int */
    ambimate_temp,
    /** Ambimate humidity, float */
    ambimate_hum,
    /** Ambimate Eco2, int */
    ambimate_Eco2,
    /** Ambimate VOC, int */
    ambimate_Voc,
    /** AS7262_Color, json */
    AS7262_Color,
    /** TSL2591_Spectrum, json */
    TSL2591_Spectrum,
    /** SCD30 CO2, int */
    SCD30_CO2,
    /** SCD30 temperature, int */
    SCD30_temp,
    /** SCD30 humidity, float */
    SCD30_hum,
    /** Audio int */
    // TODO: still need to implement some kind of data parsing as currently the data is meaningless.
    MAX4466_Audio,
    /** Oxygen int */
    MIX8410_O2,
    /** PM10 measurement. */
    PM10,
    /** PM25 measurement. */
    PM25,
    /** PM100 measurement. */
    PM100
};

/** @} */

/**
 * @brief Attributes array containing the MQTT names of the measured properties
 */
static const char * const attribute_names[] = {
  [CONN]              = "Connection",
  [ambimate_temp]     = "ambimate_Temperatuur",
  [ambimate_hum]      = "ambimate_Humidity",
  [ambimate_Eco2]     = "ambimate_Eco2",
  [ambimate_Voc]      = "ambimate_Voc",
  [AS7262_Color ]     = "AS7262_Color",
  [TSL2591_Spectrum]  = "TSL2591_Spectrum",
  [SCD30_CO2]         = "SCD30_CO2",
  [SCD30_temp]        = "SCD30_Tempratuur",
  [SCD30_hum]         = "SCD30_Humidity",
  [MAX4466_Audio]     = "MAX4466_Audio",
  [MIX8410_O2]        = "MIX8410_O2",
  // TODO: change IOT names to PM10 and PM100
  [PM10]              = "particlesPM1",
  [PM25]              = "particlesPM2_5",
  [PM100]             = "particlesPM10"
};

/**
 * @brief MQTTClient class managing the MQTT connection to the IoT.
 */
class MQTTClient {
private:
  /** @brief the ID of the sensebox module. Should be defined beforehand on the IoT platform. */
	char asset_id[100];
  /** @brief MQTT username used to log into the IoT platform. */
	char mqtt_username[100];
  /** @brief MQTT password used to log into the IoT platform. */
	char mqtt_password[100];
  /** @brief Client ID for the MQTT broker. */
	char client_id[100];
  /** @brief MQTT server port. */
	int mqtt_port;

  /** @brief  network name. */
	char ssid[100];
  /** @brief  network password. */
	char password[100];
  
  /** @brief IP of the IoT server. */
  IPAddress server;

  // json serialization buffer - Big buffer for CA cert - not used for now
  //char buffer[4096];

  /** @brief ESP WiFi client handle. */
  WiFiClientSecure espClient;
  /** @brief MQTT Client handle. */
  PubSubClient client;

  /** @brief Initializes the WiFi on the ESP32. */
  ERR_Type initWiFi();
  /** @brief Initializes the MQTT connection to the IoT platform. */
  ERR_Type initMqttConnect();
  /** @brief Reads settings from MQTTSettings.dat on the SD card
   *  @param path the path to the settings file.
   */ 
  ERR_Type getSettings(char* path);
  
  /** @brief MQTT callback function. */
  static void callback(char *topic, byte *payload, unsigned int length);

public:
  /**
   * @brief Construct a new MQTTClient object
   */
  MQTTClient();
  /**
   * @brief Destroy the MQTTClient object
   */
  ~MQTTClient();

  /**
   * @brief Initialize the MQTT and WiFi on the ESP32.
   * @param path the path to the settings file.
   */
  ERR_Type init(char* path);

  /**
   * @brief Sends data to the IoT platform.
   * 
   * @param attribute The name of the attribute on the IoT platform.
   * @param value The value to send to the IoT platform. Should be converted to a C style string.
   */
  void sendData(const char *attribute, const char *value);
  /**
   * @brief Receive data about an IoT platform. Currently not implemented.
   * 
   * @param attribute The name of the attribute to receive data about.
   */
  void receiveData(const char *attribute);
  
  /**
   * @brief Keeps the MQTT client alive.
   * Should be called in the end of the loop function.
   */
  void loopClient();
};