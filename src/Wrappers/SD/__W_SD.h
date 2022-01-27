/**
 * @file __W_SD.h
 * @author Imre Korf
 * @brief Wrapper for the SD card hardware
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once


#include "../__W_Module/__iW_Module.h"
#include "../Singleton/Singleton.h"

#include <FS.h>
#include <SD.h>

/**
 * @brief Singleton SD module.
 * 
 */
class __W_SD : public __iW_Module, public iSingleton {
private:
	/** SD library handle */
	SDFS* ESP_SD; 

	/**
	 * @brief virtual implementation of the iW_Module function.
	 * Will report an error if not initialized.
	 * @return true SD has been initialized.
	 * @return false SD has not been initialized.
	 */
	virtual bool checkInitialized();

	// remove access to the constructor of __W_SD.
	__W_SD(){}

public:
	/**
	 * @brief Get the singleton instance of the class
	 * This function makes sure that only one instance is created and accessible during runtime.
	 * @return __W_SD& the handle to the singleton instance
	 */
	static __W_SD& getInstance(){
		static __W_SD Instance;	// will only be destroyed on program exit.
		return Instance;
	}
public:
	// Assure that only one instance can exist by removing copy and assign functions.
	__W_SD(__W_SD const&)			= delete;	// delete copy constructor.
	void operator=(__W_SD const&)	= delete;	// remove assignment operator.

	/**
	 * @brief Initializes the SD object. Should only be called once.
	 * This function initializes the SD object. It has a check build in to see if this function has already been called before. If so it will just return 0.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type init();

	/**
	 * @brief lists the directories in given path.
	 * 
	 * @param dirname the path to the directory.
	 * @param levels  the depth of the tree search.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type listDir(const char* dirname, uint8_t levels);
	/**
	 * @brief Create a directory at the given path.
	 * /path/to/dir will create directory 'dir' in the folder 'to'.
	 * 
	 * @param path the path to the directory. 
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type createDir(const char* path);
	/**
	 * @brief Removes the directory at the given path.
	 * 
	 * @param path the path to the directory.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type removeDir(const char* path);
	/**
	 * @brief Gets the size of the file at the given path.
	 * 
	 * @param path the path to the file.
	 * @param val the size of the file
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type getFileSize(const char* path, unsigned long& val);
	/**
	 * @brief Reads the file at the given path into the passed buffer.
	 * 
	 * @param path the path to the file.
	 * @param buffer the buffer that is read into.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type readFile(const char * path, char* buffer);
	/**
	 * @brief rewrites the message to the given file.
	 * 
	 * @param path the path to the file.
	 * @param message the message to be written to the file.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type writeFile(const char * path, const char * message);
	/**
	 * @brief Adds the message at the end of the file.
	 * 
	 * @param path the path to the file.
	 * @param message the message to be added to the file.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type appendFile(const char * path, const char * message);
	/**
	 * @brief renames the file to the given name.
	 * e.g. "/path/to/file" "/path/to/file2" rewrites "file" to "file2".
	 * @param path1 the original path to the file.
	 * @param path2 the new path to the file.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type renameFile(const char * path1, const char * path2);
	/**
	 * @brief deletes the file at the given path.
	 * 
	 * @param path path to the file.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type deleteFile(const char * path);

	/**
	 * @brief tests the IO functionality of the SDcard.
	 * 
	 * @param path to a file to test.
	 * @return ERR_Type returns SUCCESS on succesfull exit. Else it will return an error code.
	 * @see ERR_Type
	 */
	ERR_Type testFileIO(const char * path);
};