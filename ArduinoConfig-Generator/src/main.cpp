#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <conio.h>
#include <stdlib.h>
#include <cctype>
#include <string>

#define KEY 4181456146874

struct WifiSettings {
	std::string asset_id;
	std::string mqtt_username;
	std::string mqtt_password;
	std::string client_id;
	uint16_t mqtt_port;
	union {
		uint8_t IParr[4];
		struct {
			uint8_t IP_byte1;
			uint8_t IP_byte2;
			uint8_t IP_byte3;
			uint8_t IP_byte4;
		};
	};
	std::string SSID;
	std::string Network_password;
	// newvariable
};

bool getWifiConfig(WifiSettings& WFS);
bool checkUserInput(std::string input, bool recursive = false);

int main(){
	// allocate the WFS.
	WifiSettings WFS;
	while(!getWifiConfig(WFS));

	/*
	WFS.asset_id = "/6s1BZTA6OQN21yzYjEcHV6";
	WFS.mqtt_username = "alkmaar:mqtt_sensbox";
	WFS.mqtt_password = "55c151c7-67dd-4533-8c4e-3bdbf0e84129";
	WFS.client_id = "SenseBox_test";
	WFS.mqtt_port = 8883;
	WFS.IParr[0] = 51;
	WFS.IParr[1] = 144;
	WFS.IParr[2] = 101;
	WFS.IParr[3] = 191;
	WFS.SSID = "SSID";
	WFS.Network_password = "PASSWORD";
	*/

	std::ofstream output;
	output.open("MQTTSettings.dat", std::fstream::binary);

	std::stringstream output_stream;

	output_stream << WFS.asset_id << std::endl;
	output_stream << WFS.mqtt_username << std::endl;
	output_stream << WFS.mqtt_password << std::endl;
	output_stream << WFS.client_id << std::endl;
	output_stream << WFS.mqtt_port << std::endl;
	output_stream << (int)WFS.IParr[0] << "." << (int)WFS.IParr[1] << "." << (int)WFS.IParr[2] << "." << (int)WFS.IParr[3] << std::endl;
	output_stream << WFS.SSID << std::endl;
	output_stream << WFS.Network_password << std::endl;
	// new variable EXAMPLE:
	// output_stream << WFS.newvariable << std::endl;



	// perform encryption
	size_t stringlength = output_stream.str().length();
	for (size_t i = 0; i < stringlength; i++)
	{	
		char convert = output_stream.str()[i];
		uint8_t crypt = (static_cast<size_t>(convert) + i + (KEY % 255) + stringlength) % 255;
		uint8_t mult = (static_cast<size_t>(convert) + i + (KEY % 255) + stringlength) / 255;
		uint16_t val = mult << 8 | crypt;
		
		output.write((char*)&val, sizeof(uint16_t));
	}
	// signify end of file.
	char eof = 0x05;
	output.write(&eof, sizeof(char));

	output.close();

	std::cout << std::endl;

	return 0;
}


bool getWifiConfig(WifiSettings& WFS){
	bool valid = true;
	// asset id //
	do{
		std::cout << "Please provide the asset id for the SenseBox." << std::endl;
		std::cout << "asset_id: /"; std::cin >> WFS.asset_id; std::cout<< std::endl;
		WFS.asset_id.insert(WFS.asset_id.begin(), '/');
	}while(!checkUserInput(WFS.asset_id));
	// mqtt username //
	do{
		std::cout << "Please provide the mqtt username for the SenseBox. [location:name]" << std::endl;
		std::cout << "mqtt username: "; std::cin >> WFS.mqtt_username; std::cout<< std::endl;
	}while(!checkUserInput("" + WFS.mqtt_username));
	// mqtt password // 
	do{
		std::cout << "Please provide the mqtt_password for the SenseBox." << std::endl;
		std::cout << "mqtt password: "; std::cin >> WFS.mqtt_password; std::cout<< std::endl;
	}while(!checkUserInput("" + WFS.mqtt_password));
	// client id //
	do{
		std::cout << "Please provide the client id for the SenseBox." << std::endl;
		std::cout << "client id: "; std::cin >> WFS.client_id; std::cout<< std::endl;
	}while(!checkUserInput("" + WFS.client_id));
	// MQTT port //
	std::string port;
	do{
		valid = true;
		std::cout << "Please provide the mqtt port for the SenseBox. [integer]" << std::endl;
		std::cout << "mqtt port: "; std::cin >> port; std::cout<< std::endl;
		try{
			for(std::size_t i = 0; i < port.length(); i++){
				if(!isdigit(port[i])){
					throw std::invalid_argument("");
				}
			}
			WFS.mqtt_port = std::atoi(port.c_str());
		}
		catch(std::invalid_argument const& ex){
			std::cout << "Input should be an integer." << std::endl;
			valid = false;
			continue; // skip the check userinput part.
		}
	}while(!valid || !checkUserInput("" + std::to_string(WFS.mqtt_port)));
	// SSID //
	do{
		std::cout << "Please provide the SSID of the network the sensebox should connect to." << std::endl;
		std::cout << "SSID: "; std::cin >> WFS.SSID; std::cout<< std::endl;
	}while(!checkUserInput("" + WFS.SSID));
	// network password //
	do{
		valid = true;
		std::cout << "Please provide the password of the network the sensebox should connect to." << std::endl;
		std::cout << "password: "; 
		std::string password;
		int key = 0;
		while(key!=13 )  // Enter 
		{
			key = getch();
			if(key == 8 ){ // Backspace
				password.erase(password.end()-1);
			}
			// check if key is valid ASCII
			else if(key >= 0x20 && key <= 0x7E){
				password += key;
			}
		}
		std::cout << std::endl << "re-enter password: "; 
		std::string password2;
		key = 0;
		while(key!=13 )  // Enter
		{
			key = getch();
			if(key == 8 ){  // Backspace
				password2.erase(password2.end()-1);
			}
			// check if key is valid ASCII
			else if(key >= 0x20 && key <= 0x7E){
				password2 += key;
			}
		}
		if(password2.compare(password)){
			std::cout << "password mismatch. " << std::endl;
			valid = false;
		}
		else{
			WFS.Network_password = password;
		}
		std::cout << std::endl;
	}while(!valid);
	// mqtt broker ip //
	std::string IP;
	std::string IPcopy;
	do{
		valid = true;
		std::cout << "Please provide the mqtt broker ip for the SenseBox. [ip1.ip2.ip3.ip4]" << std::endl;
		std::cout << "mqtt broker ip: "; std::cin >> IP; std::cout<< std::endl;
		IPcopy = IP;
		size_t n = std::count(IP.begin(), IP.end(), '.'); // get occurrence count of .
		// check if 3 seperators are provided. 
		if(n == 3){
			int i = 3;
			while(i > 0 && valid){
				std::size_t found = IP.rfind("."); // find last . and get substring
				if(found !=std::string::npos){
					std::string byte = IP.substr(found + 1);
					try{
						for(std::size_t i = 0; i < byte.length(); i++){
							if(!isdigit(byte[i])){
								throw std::invalid_argument("");
							}
						}
						WFS.IParr[i] = std::stoi(byte);
					}
					catch(std::invalid_argument const& ex){
						std::cout << "Input should be an integer." << std::endl;
						valid = false;
						continue; // skip the check userinput part.
					}
					IP = IP.substr(0, found);
					i--;
				}
			}
			if(!valid){
				continue;
			}
			try{
				WFS.IParr[i] = std::stoi(IP);
			}
			catch(std::invalid_argument const& ex){
				std::cout << "Input should be an integer." << std::endl;
				valid = false;
				continue; // skip the check userinput part.
			}			
		}
		else{
			std::cout << "invalid ip address. " << std::endl;
			valid = false;
			continue; // skip the check userinput part.
		}
	}while(!valid || !checkUserInput("" + IPcopy));

	// example new variable setting
	/*
	do{
		// get user input
		// assign to a variable
		// if need be the "valid" variable can be used to add more security ( see MQTT port for an example )
	}while(!checkUserInput("" + string));
	*/

	return true;
}

bool checkUserInput(std::string input, bool recursive){
	if(!recursive){
		std::cout << "You entered: " << input << std::endl;
	}
	std::string confirm;
	std::cout << "Is this correct? [y\\n] "; 
	int key = getch();
	std::cout << std::endl;
	if(key == 'y' || key == 13 /* Enter */){
		return true;
	}
	else if(key == 'n'){
		return false;
	}
	else{
		std::cout << "please provide a \"y\" or an \"n\"." << std::endl;
		// yay recursive
		return checkUserInput(input, true);
	}
	
}