#ifndef __VOTABLE__
#define __VOTABLE__

#include <vector>
#include <string>

using std::string;
using std::vector;

class VoTable{

public:

	VoTable();
	
	void set_tableName(const string & table_name){ this->table_name = table_name; }
	void set_fields(const vector<string> & fields) { this->fields = fields; }
	void set_values(const vector<string> & values) { this->values = values; }

	string get_tableName()const{ return table_name; }
	vector<string> get_fields()const{ return fields; }
	vector<string> get_values()const{ return values; }

private:

	string table_name;
	vector<string> fields;
	vector<string> values;


};




#endif
