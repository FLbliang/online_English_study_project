#include <string>

#include "database.h"
#include "create_database.h"

using std::string;

string CreateDatabase::user = "root";
string CreateDatabase::password = "root";
string CreateDatabase::host = "127.0.0.1";
string CreateDatabase::database = "crazy_study_English";

DataBase * CreateDatabase::get_database(){
	return new DataBase(
					CreateDatabase::user.data(), 
					CreateDatabase::password.data(), 
					CreateDatabase::host.data(), 
					CreateDatabase::database.data()
			);
}
