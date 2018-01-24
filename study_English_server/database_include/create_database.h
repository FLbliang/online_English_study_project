#ifndef __CREATEDATABASE__
#define __CREATEDATABASE__

#include "database.h"
#include <string>

using std::string;

class CreateDatabase{
public:
	static string host;
	static string user;
	static string password;
	static string database;

public:
	static DataBase * get_database(); 
	
};



#endif
