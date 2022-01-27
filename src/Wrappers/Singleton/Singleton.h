/**
 * @file Singleton.h
 * @author Imre Korf
 * @brief Singleton template interface
 * @version 0.1
 * @date 2021-12-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/**
 * @brief Singleton template
 * this doesn't actually implement anything, but this is more for keeping track if a class is a singleton.
 * Should assure that only one instance of the class can exist at a time.
 */
class iSingleton {
/*
public:
	// make sure that only one instance is created and accessible during runtime
	static Derived& getInstance(){
		static Derived Instance;	// will only be destroyed on program exit
		return Instance;
	}
private:	
	// remove access to the constructor of derived
	Derived(){}
public:
	// Assure that only one instance can exist by removing copy and assign functions
	Derived(Derived const&)			= delete;	// delete copy constructor
	void operator=(Derived const&)	= delete;	// remove assignment operator
*/
};