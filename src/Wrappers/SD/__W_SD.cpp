#include "__W_SD.h"
#include <SPI.h>
#include "../../Logger/Logger.h"


bool __W_SD::checkInitialized(){
    if(Initialized){
        return 0;
    }
    else{
        Logger::getInstance().println("SD card is not initialized!", LogLevel::Error, LogType::Serial);
        return 1;
    }
}

ERR_Type __W_SD::init(){
    if(Initialized){return ALREADY_INITIALIZED;}	// return if already initialized;
	ESP_SD = &SD; // rename the SD handle and save it in this class.
    // check if the SD card is inserted.
    if(digitalRead(25)){
        Logger::getInstance().println("SD-card not inserted!", LogLevel::Error, LogType::Serial);
        return NO_SD_CARD;
    }
    // start the SD module
	if(!ESP_SD->begin()){
        Logger::getInstance().println("SD Card Mount Failed", LogLevel::Error, LogType::Serial);
        return SD_BEGIN_ERR;
    }
    // check the cardtype
	uint8_t cardType = ESP_SD->cardType();

	if(cardType == CARD_NONE){
        Logger::getInstance().println("No SD card attached", LogLevel::Error, LogType::Serial);
        return SD_CARDTYPE_NONE;
    }
	Initialized = true; // SD card is not properly initialized, so it's cool to start printing to it. ( prevents recursive calls to checkInitialized etc. )

    // debug info
	Logger::getInstance().print("SD Card Type: ", LogLevel::Info, LogType::Serial);
    if(cardType == CARD_MMC){
        Logger::getInstance().println("MMC", LogLevel::Info, LogType::Serial);
    } else if(cardType == CARD_SD){
        Logger::getInstance().println("SDSC", LogLevel::Info, LogType::Serial);
    } else if(cardType == CARD_SDHC){
        Logger::getInstance().println("SDHC", LogLevel::Info, LogType::Serial);
    } else {
        Logger::getInstance().println("UNKNOWN", LogLevel::Info, LogType::Serial);
    }
	
    // more debug info
	uint32_t cardSize = ESP_SD->cardSize() / (1024 * 1024);
    Logger::getInstance().println((String)("SD Card Size: " + String(cardSize) + "MB"), LogLevel::Info, LogType::Serial);

	return SUCCESS;
}

ERR_Type __W_SD::listDir(const char * dirname, uint8_t levels){
    if(checkInitialized()){return NOT_INITIALIZED;} // don't act on to the SD hardware if not properly intialized;

    Logger::getInstance().println((String)("[SD] Listing directory: " + String(dirname)), LogLevel::Info);

    // open the file
    File root = ESP_SD->open(dirname);
    if(!root){
        Logger::getInstance().println("[SD] Failed to open directory", LogLevel::Error);
        return SD_DIR_OPEN_FAIL;
    }

    if(!root.isDirectory()){
        Logger::getInstance().println("[SD] Not a directory", LogLevel::Error);
        return SD_NOT_A_DIR;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Logger::getInstance().print("  DIR : ", LogLevel::Info);
            Logger::getInstance().println(file.name(), LogLevel::Info);
            // re-execute the function with 1 less level. (recursive)
            if(levels){
                listDir(file.name(), levels -1);
            }
        } else {
            Logger::getInstance().print("  FILE: ", LogLevel::Info);
            Logger::getInstance().print(file.name(), LogLevel::Info);
            Logger::getInstance().print("  SIZE: ", LogLevel::Info);
            Logger::getInstance().println(file.size(), LogLevel::Info);
        }
        file = root.openNextFile();
    }
    return SUCCESS;
}

ERR_Type __W_SD::createDir(const char * path){
    if(checkInitialized()){return NOT_INITIALIZED;} // don't act on to the SD hardware if not properly intialized;

    Logger::getInstance().println((String)("[SD] Creating Dir: " + String(path)), LogLevel::Info);
    if(ESP_SD->mkdir(path)){
        Logger::getInstance().println("[SD] Dir created", LogLevel::Info);
        return SUCCESS;
    } else {
        Logger::getInstance().println("[SD] mkdir failed", LogLevel::Error);
        return SD_MKDIR_FAIL;
    }
}

ERR_Type __W_SD::removeDir(const char * path){
    if(checkInitialized()){return NOT_INITIALIZED;} // don't act on to the SD hardware if not properly intialized;

    Logger::getInstance().println(String("[SD] Removing Dir: " + String(path)), LogLevel::Info);
    if(ESP_SD->rmdir(path)){
        Logger::getInstance().println("[SD] Dir removed", LogLevel::Info);
        return SUCCESS;
    } else {
        Logger::getInstance().println("[SD] rmdir failed", LogLevel::Warning);
        return SD_RMDIR_FAIL;
    }
}

ERR_Type __W_SD::getFileSize(const char* path, unsigned long& val){
    if(checkInitialized()){return NOT_INITIALIZED;} // don't act on to the SD hardware if not properly intialized;
    Logger::getInstance().println(String("[SD] Getting filesize of: " + String(path)), LogLevel::Info);
    File file = ESP_SD->open(path);
    if(!file){
        Logger::getInstance().println("[SD] Failed to open file for reading", LogLevel::Error);
        file.close();
        return SD_FILE_OPEN_FAIL;
    }
    
    val = file.size();
    file.close();

    return SUCCESS;

}

ERR_Type __W_SD::readFile(const char * path, char* buffer){
    if(checkInitialized()){return NOT_INITIALIZED;} // don't act on to the SD hardware if not properly intialized;
    
    Logger::getInstance().println(String("[SD] Reading file: " + String(path)), LogLevel::Info);

    File file = ESP_SD->open(path);
    if(!file){
        Logger::getInstance().println("[SD] Failed to open file for reading", LogLevel::Error);
        file.close();
        return SD_FILE_OPEN_FAIL;
    }

    Logger::getInstance().println("[SD] Read from file: ", LogLevel::Info);
    // read the file into the buffer and print to the logger
    uint64_t i = 0;
    while(file.available()){
        buffer[i++] = file.read();
        Logger::getInstance().write(buffer[i-1], LogLevel::DataDump);
    }
    file.close();
	Logger::getInstance().dataDumpEnd(LogType::Serial_SD);
    return SUCCESS;
}

ERR_Type __W_SD::writeFile(const char * path, const char * message){
    if(checkInitialized()){return NOT_INITIALIZED;} // don't act on to the SD hardware if not properly intialized;

    Logger::getInstance().println(String("[SD] Writing file: " + String(path)), LogLevel::SD_printInfo, LogType::Serial);

    File file = ESP_SD->open(path, FILE_WRITE);
    if(!file){
        Logger::getInstance().println("[SD] Failed to open file for writing", LogLevel::Error, LogType::Serial);
        return SD_FILE_OPEN_FAIL;
    }
    if(file.print(message)){
        Logger::getInstance().println("[SD] File written", LogLevel::SD_printInfo, LogType::Serial);
    } else {
        Logger::getInstance().println("[SD] Write failed", LogLevel::Warning, LogType::Serial);
        file.close();
        return SD_WRITE_FAIL;
    }
    file.close();
    return SUCCESS;
}

ERR_Type __W_SD::appendFile(const char * path, const char * message){
    if(checkInitialized()){return NOT_INITIALIZED;} // don't act on to the SD hardware if not properly intialized;

    Logger::getInstance().println(String("[SD] Appending to file: " + String(path)), LogLevel::SD_printInfo, LogType::Serial);

    File file = ESP_SD->open(path, FILE_APPEND);
    if(!file){
        Logger::getInstance().println("[SD] Failed to open file for appending", LogLevel::Error, LogType::Serial);
        return SD_FILE_OPEN_FAIL;
    }
    if(file.print(message)){
        Logger::getInstance().println("[SD] Message appended", LogLevel::SD_printInfo, LogType::Serial);
        return SUCCESS;
    } else {
        Logger::getInstance().println("[SD] Append failed", LogLevel::Warning, LogType::Serial);
        file.close();
        return SD_APP_FAIL;
    }
    file.close();
    return SUCCESS;
}

ERR_Type __W_SD::renameFile(const char * path1, const char * path2){
    if(checkInitialized()){return NOT_INITIALIZED;} // don't act on to the SD hardware if not properly intialized;

    Logger::getInstance().println(String("[SD] Renaming file " + String(path1) + " to " + String(path2)), LogLevel::Info);
    if (ESP_SD->rename(path1, path2)) {
        Logger::getInstance().println("[SD] File renamed", LogLevel::Info);
        return SUCCESS;
    } else {
        Logger::getInstance().println("[SD] Rename failed", LogLevel::Error);
        return SD_RENAME_FAIL;
    }
}

ERR_Type __W_SD::deleteFile(const char * path){
    if(checkInitialized()){return NOT_INITIALIZED;} // don't act on to the SD hardware if not properly intialized;

    Logger::getInstance().println(String("[SD] Deleting file: " + String(path)), LogLevel::Info);
    if(ESP_SD->remove(path)){
        Logger::getInstance().println("[SD] File deleted", LogLevel::Info);
        return SUCCESS;
    } else {
        Logger::getInstance().println("[SD] Delete failed", LogLevel::Warning);
        return SD_RM_FAIL;
    }
}

ERR_Type __W_SD::testFileIO(const char * path){
    if(checkInitialized()){return NOT_INITIALIZED;} // don't act on to the SD hardware if not properly intialized;
    
    Logger::getInstance().println("[SD] Testing FileIO", LogLevel::Info);

    // open a file and read 512 bytes. check how long it took and print that time.
    File file = ESP_SD->open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Logger::getInstance().println(String(String((uint32_t)flen) + " bytes read for " + String(end) + " ms"), LogLevel::Info);
        file.close();
    } else {
        Logger::getInstance().println("[SD] Failed to open file for reading", LogLevel::Error);
        return SD_FILE_OPEN_FAIL;
    }

    // open a file and write 2048 * 512 bytes. check how long it took and print that time.
    file = ESP_SD->open(path, FILE_WRITE);
    if(!file){
        Logger::getInstance().println("[SD] Failed to open file for writing", LogLevel::Error);
        file.close();
        return SD_FILE_OPEN_FAIL;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
	Logger::getInstance().println(String(String((uint32_t)2048 * 512) + " bytes written for " + String(end) + " ms"), LogLevel::Info);
    file.close();
    return SUCCESS;
}