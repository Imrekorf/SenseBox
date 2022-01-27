#include "MQTT.h"
#include "../Logger/Logger.h"
#include <Arduino.h>

#define KEY 4181456146874

MQTTClient::MQTTClient() : client(espClient){

}

MQTTClient::~MQTTClient(){

}

ERR_Type MQTTClient::init(char* path){
	ERR_Type ret; 
	if(ret = getSettings(path), ret){
		return ret;
	}
	if(ret = initWiFi(), ret){
		return ret;
	}
	if(ret = initMqttConnect(), ret){
		return ret;
	}
	return SUCCESS;
}

ERR_Type MQTTClient::getSettings(char* path){
	// read settings
	unsigned long length;
	__W_SD::getInstance().getFileSize(path, length);
	if(length == 0){
		Logger::getInstance().println(String(path) + " is empty.", LogLevel::Error);
		return WIFI_SETTINGS_EMPTY;
	}

	// create a buffer
	char* buffer = new char[length]; // spooky mem alloc on a mcu
	__W_SD::getInstance().readFile(path, buffer);

	// decrypt
	char* settings = new char[(length-1)/2+1];
	for (unsigned long i = 0; i < length-1; i+=2)
	{
		uint8_t crypt = buffer[i];
		uint8_t mult = buffer[i+1];
		settings[i/2] = (char)(((mult * 255) + crypt) - ((length-1) / 2) - (KEY % 255) - (i / 2)); // math magic
	}
	settings[(length-1)/2] = '\0'; // signify end of string of the buffer.

	// format
	// ! add any other settings that should be changed by the MQTTSettings file here. 
	// ! Anything that should end up as a char* and is under 50 characters will be auto converted
	char* arr[] = {
		asset_id, mqtt_username, mqtt_password, client_id, nullptr, nullptr, ssid, password
		//, other_variable <- can be anything as long as you can parse it from a character array :), 
		// just make sure it's a pointer to the character array. or a nullptr if it's a special conversion. 
		// ( could also store other pointer types in here through C casting magic though ) ps. don't do it.
		}; // add other settings here

	unsigned int j = 0; // settings string counter
	for(int i = 0; i < (sizeof(arr)/sizeof(arr[0])); i++){
		unsigned int k = 0; // max 50; character counter
		// add any special conversion here. (Special being anything that isnt a string), check "// parse port" for an example
		/* // ! example special parse
		if(i == n){
			// some safety and logging
			if(settings[j] == '\0'){
				Logger::getInstance().println("premature exit from config formatting", LogLevel::Error);
				return WIFI_SETTINGS_NOT_COMPLETE;
			}
			// special parsing
			char buff[N]; // some buffer to store the value in temporarily
			while(settings[j] != '\n'){
				// more safety and logging
				if(settings[j] == '\0'){
					Logger::getInstance().println("premature exit from config formatting", LogLevel::Error);
					return WIFI_SETTINGS_NOT_COMPLETE;
				}
				// copy from settings array to temporary buffer
				buff[k++] = settings[j++];
			}
			j++; // increase j to the next settings character to prevent infinite loop.
			buff[k] = '\0'; // end the buffer c style string. For safety
			variable_to_be_set = atoi(buff); // convert text to int
		}
		*/
		// parse port
		if(i == 4){
			if(settings[j] == '\0'){
				Logger::getInstance().println("premature exit from config formatting", LogLevel::Error);
				return WIFI_SETTINGS_NOT_COMPLETE;
			}
			char buff[10]; // character port buffer
			while(settings[j] != '\n'){
				if(settings[j] == '\0'){
					Logger::getInstance().println("premature exit from config formatting", LogLevel::Error);
					return WIFI_SETTINGS_NOT_COMPLETE;
				}
				buff[k++] = settings[j++];
			}
			j++;
			buff[k] = '\0'; // add string ender
			mqtt_port = atoi(buff); // convert
		}
		// parse IP
		else if(i == 5){
			if(settings[j] == '\0'){
				Logger::getInstance().println("premature exit from config formatting", LogLevel::Error);
				return WIFI_SETTINGS_NOT_COMPLETE;
			}
			char buff[4]; // octal buffer
			uint8_t octals[4]; // integer octal buffer
			unsigned int l = 0; // octal counter
			while(settings[j] != '\n'){
				if(settings[j] != '.'){ // check if octal has ended
					buff[k++] = settings[j++];
				}
				else{
					buff[k] = '\0'; // end octal string
					octals[l++] = atoi(buff); // convert
					j++;
					k = 0;
				}
			}
			j++;
			// add last octal
			buff[k] = '\0'; // end octal string
			octals[l++] = atoi(buff); // convert
			server = IPAddress(octals);
		}
		// parse settings which are strings
		else {
			while(settings[j] != '\n'){
				// parse other settings
				if(settings[j] == '\0'){
				Logger::getInstance().println("premature exit from config formatting", LogLevel::Error);
				return WIFI_SETTINGS_NOT_COMPLETE;
			}
				if(k >= 100){
					Logger::getInstance().println("Provided settings string is over 100 characters.", LogLevel::Error);
					return WIFI_SETTINGS_STRING_OVERLOAD;
					break;
				}
				arr[i][k++] = settings[j++]; // add value to the character array.
			}
			j++;
			arr[i][k] = '\0';
		}
	}
	return SUCCESS;
}

ERR_Type MQTTClient::initWiFi() {
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	Logger::getInstance().print("Connecting to WiFi ..", LogLevel::Info);

	int i = 0;
	for(; i < WIFI_TIMEOUT && WiFi.status() != WL_CONNECTED; i++) {
		Logger::getInstance().print('.', LogLevel::Info);
		delay(1000);
	}

	if(i >= WIFI_TIMEOUT){
		Logger::getInstance().println("\n Failed to connect to WiFi", LogLevel::Error);
		return WIFI_CONN_FAIL;
	}
	else{
		Logger::getInstance().print("\nConnencted to wiFi with ip: ", LogLevel::Info);
		Logger::getInstance().print(WiFi.localIP()[0], LogLevel::Info); Logger::getInstance().print(".", LogLevel::Info);
		Logger::getInstance().print(WiFi.localIP()[1], LogLevel::Info); Logger::getInstance().print(".", LogLevel::Info);
		Logger::getInstance().print(WiFi.localIP()[2], LogLevel::Info); Logger::getInstance().print(".", LogLevel::Info);
		Logger::getInstance().print(WiFi.localIP()[3], LogLevel::Info); Logger::getInstance().println("", LogLevel::Info);
		return SUCCESS;
	}
}

ERR_Type MQTTClient::initMqttConnect()
{
	Logger::getInstance().println("\nStarting connection to server...", LogLevel::Info);
	if(!WiFi.isConnected()){
		Logger::getInstance().println("Not initializing the MQTT broker due to no WiFi connection.", LogLevel::Warning);
		return WIFI_CONN_FAIL;
	}
	// !IMPORTANT! If the following line is not set the server won't connect. Without this line it will try to verify a CA cert, that we don't have
	espClient.setInsecure();

	//  Set buffer size for autoprovision
	//  if(!client.setBufferSize(4096))
	//  {
	//    Serial.println("the buffer could not be resized");
	//  }
	client.setServer(server, mqtt_port);
	client.setCallback(callback);

	int retries = 0;
	while (!client.connected() && retries < MQTT_CONN_TIMEOUT) {
		Logger::getInstance().println("The client " + String(client_id) + " connects to the public mqtt broker", LogLevel::Info);
		if (client.connect(client_id, mqtt_username, mqtt_password)) {
				Logger::getInstance().println("Mqtt broker connected", LogLevel::Info);
		} else {
				retries++;
				Logger::getInstance().print("failed with state ", LogLevel::Error);
				Logger::getInstance().println(client.state(), LogLevel::Error);
				delay(2000);
		}
	}
	if(retries >= MQTT_CONN_TIMEOUT){
		return MQTT_CONN_FAIL;
	}
	// Allocate JsonDocument
	//  const int capacity = JSON_OBJECT_SIZE(2);
	//  StaticJsonDocument<capacity> doc;
	//
	//  // Add the objects
	//  doc["type"] = "x509";
	//  doc["cert"] = "-----BEGIN CERTIFICATE-----\nMIIF8zCCA9sCFDmx9YwAvYOH/0v9O5E8KwMR2MkLMA0GCSqGSIb3DQEBCwUAMIG1MQswCQYDVQQGEwJOTDEWMBQGA1UECAwNTm9vcmQtSG9sbGFuZDEQMA4GA1UEBwwH\nQWxrbWFhcjEdMBsGA1UECgwUSG9nZXNjaG9vbCBJbkhvbGxhbmQxHzAdBgNVBAsM\nFlRlY2huaXNjaGUgSW5mb3JtYXRpY2ExEDAOBgNVBAMMB1RpbG1hbm4xKjAoBgkq\nhkiG9w0BCQEWG1RpbG1hbm4uS29zdGVyQGluaG9sbGFuZC5ubDAeFw0yMTExMjky\nMTAyNDlaFw0yMjExMjkyMTAyNDlaMIG1MQswCQYDVQQGEwJOTDEWMBQGA1UECAwN\nTm9vcmQtSG9sbGFuZDEQMA4GA1UEBwwHQWxrbWFhcjEdMBsGA1UECgwUSG9nZXNj\naG9vbCBJbkhvbGxhbmQxHzAdBgNVBAsMFlRlY2huaXNjaGUgSW5mb3JtYXRpY2Ex\nEDAOBgNVBAMMB1RpbG1hbm4xKjAoBgkqhkiG9w0BCQEWG1RpbG1hbm4uS29zdGVy\nQGluaG9sbGFuZC5ubDCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBALlw\n6OvDFkA0P8Bg4kVwYpU0DW7DiKp0AQ/bYV/8Dg+9GBrr4Ck2q/m8jw8V2+FYUINp\nnVWo5CYiVy15QfDa5HgPFOzMDkTw3cstdOnLtz82hZePg1yLePD7wYJYF+NPZOoR\n5bPCRwjkKn5+c+mldKskNaEQLKQktLg1NFZCLjfsbilxzdbyjenP82/7gz6SK1GH\nvgsm6HDDXq3HqjIUk2dqjBCVDjSIiTwifq18JMmM4bJKhN+1VRxq+njiEuiZ063C\nUbxq0I3feIgwW49fOyvMCxnv8G+NO5crD6O4tWtT2eK8TtuhRJMFnH9vtXtQS5sz\nPZUrHCm7UHyuhxkeivzlAaAm/tv7Lzs7GB56y0+/H/e8KBQAgAUuodg4MyM4wXqQ\nvOCNpz6zt20YzV0aA/3cF2NQg/61h8GmhgbO0r932KaG+RO6SjLZuBi7IF+M7VYi\nF7XdZBEWREFS8rsFz8TBQWl+Ay6ARSFoq93dgKrAook050tCI3TxjBI3x7JndEQk\nPuAY56lnynihvstS6tWVXBcYT0rWGaJUcQ02rnkbYP4IwKLTazIk88/echw2QXs4\nhylbXBKlhfV3CnonGOSvyF8LqlHtGAQg9cKIl1bmYyPtlsY0qkB1GFDLkKPEz2qj\n9DC3ElOolRWNiaQ+UthIY/eliSu1mgUMG0tOzEd1AgMBAAEwDQYJKoZIhvcNAQEL\nBQADggIBAFINP9VXFvPmzK+4C7w/FGDkZKKOl5Jw+hvWtRdtuDRhUs9aO6YmFm1d\nQ25rOwK9hC+YEY5ihyiepVM+QcygMl52nO9shxF9BnqvQGJTimd8Wl94oH1I2X98\nQ+/JCWswsxHzLqn/Q7sFZff3CgFCURxu9HIuz2aJgZv7ar9ET657aFppwWvchXuG\nNlBZLfFww6xgeMX3YqCXUP48tU4Fc1igtAmHlef1z6lA1QbMZExFFcudjYTIlFJw\nhpYc+/mkSPP5NrBLm64lA3UdCQCLBbDbLQ/ObKX7edakt5HQWjmcXv7EzGRO7Y/r\nSxps8sbM98q/A0DJRAy3MH+taGXy7e/ym9PYOOj9hxtewB3TFR5e3lDHus6533Ae\nQ8VLQHwhygrGv9woYS87r6/zKlZZA2ffBusnLgdhcJPou9GxMtTzWq3STaMJGN6m\nRI2CzZ5YXVtQwbjL5PR7umu0orBTs6k5Ponlh930ONLEGmAtKGGwvfM6caDKwlTs\n1ZGHKkCdgUi05d7CfgNl7IysHuebiA39fq/LRhw7PxzJrloLLZT1wJNJ8gCLoWpT\nHVIFZFd597nLjl8vYL/mptl88VWWsC0ELtQ7N6xY5JDhccNiWVGssmI/kzfOJ+J3\nLvKoQMkB0NCaQEEn2o+iT361B9pSPnS1/wwc1UAo3+kcAJ8yh1gn\n-----END CERTIFICATE-----";
	//
	//  serializeJsonPretty(doc, buffer);
	return SUCCESS;
}

void MQTTClient::sendData(const char *attribute, const char *value) {
	// Current Topic
	char result[100] = "alkmaar/";   // array to hold the result.

	strcat(result, client_id);
	strcat(result, "/writeattributevalue/");
	strcat(result, attribute);
	strcat(result, asset_id);

	client.publish(result, value);
}

void MQTTClient::receiveData(const char *attribute) {
	// Current Topic
	char result[100] = "alkmaar/";   // array to hold the result.

	strcat(result, client_id);
	strcat(result, "/attribute/");
	strcat(result, attribute);
	strcat(result, asset_id);

	client.subscribe(result);
}

void MQTTClient::callback(char *topic, byte *payload, unsigned int length) {
 Logger::getInstance().print("\nMessage arrived in topic: ", LogLevel::Info);
 Logger::getInstance().println(topic, LogLevel::Info);
 Logger::getInstance().print("Message:", LogLevel::DataDump);
 for (int i = 0; i < length; i++) {
		 Logger::getInstance().write((char) payload[i], LogLevel::DataDump);
 }
 Logger::getInstance().dataDumpEnd();
}

void MQTTClient::loopClient(){
	client.loop();
}