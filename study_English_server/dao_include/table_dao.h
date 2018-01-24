#ifndef __TABLEDAO__
#define __TABLEDAO__

#include <vector>
#include <string>

#include "database.h"
#include "vo_table.h"

using std::vector;
using std::string;

class TableDao{
		
public:

	TableDao();
	~TableDao();

	string get_limits(const VoTable & object, bool is_limit = true);
	string get_orderBy(const VoTable & object);

	void set_loginTime(const int & manager_id);

	bool querySearchData(const VoTable & object);
	bool queryPageData(const string & table_name, const int & page_num, const string & limit_key, const string & limit_value);
	bool insertData(const VoTable & object);
	bool deleteData(const VoTable & object);
	bool updateData(const VoTable & old_object, const VoTable & new_object);

	bool set_log(const string & log_info, const int & manager_id, const string & manager_ip);
	bool judge_object(const VoTable & object);
	bool get_tableSum(int & record_sum, const string & table_name, const string & key, const string & value);

	vector<VoTable> get_objects();

private:

	DataBase * database;
	vector<VoTable> objects;
	string table_name;

};





#endif
