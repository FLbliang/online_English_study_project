#include <unistd.h>
#include <vector>
#include <string>

#include "table_dao.h"
#include "create_database.h"
#include "utils.h"

using namespace std;

TableDao::TableDao(){

	this->database = CreateDatabase::get_database();
}

TableDao::~TableDao(){
	delete this->database;
}

string TableDao::get_limits(const VoTable & object, bool is_limit){

	vector<string> fields = object.get_fields();
	vector<string> values = object.get_values();
	
	int len = fields.size();
	string limit;
	string res_str = "";

	for(int i = 0; i < len; ++i){

		limit = (values[i].empty() ? "" : fields[i] + " = '" + values[i] + "'");

		if(!limit.empty()){

			res_str += ( i > 0 ? ( is_limit ? " and " : " , ") : "");
			res_str += limit;
		}

	}

	return res_str;	

}

void TableDao::set_loginTime(const int & manager_id){

	string sql = "update manager_info set last_login = current_timestamp where manager_id = " + to_string(manager_id) + ";";
	database->query(sql.data());	
}

bool TableDao::get_tableSum(int & record_sum, const string & table_name, const string & key, const string & value){

	string sql = "select count(*) from " + table_name;

	if(!key.empty() && !value.empty()){
		sql += " where " + key + " = '" + value + "'";
	}

	if(database->query(sql.data()) == false){
		return false;
	}

	string ** result = this->database->get_result();
	int row_len = this->database->get_rowNum();
	int col_len = this->database->get_colNum();

	if(row_len == 1 && col_len == 1 && is_int(result[0][0])){

		record_sum = stoi(result[0][0]);
		this->database->free_result();
		return true;
	}else{

		this->database->free_result();
		return false;
	}
}

bool TableDao::judge_object(const VoTable & object){

	vector<string> fields = object.get_fields();
	vector<string> values = object.get_values();

	unsigned int len = fields.size();
	if(len != values.size()){
		return false;
	}

	for(unsigned int i = 0; i < len; ++i){

		if(fields[i].compare("int") == 0 && !is_int(values[i])){
			return false;	
		}else if(fields[i].compare("double") == 0 && !is_float(values[i])){
			return false;
		}

			
	}

	return true;

}

bool TableDao::querySearchData(const VoTable & object){

	if(!judge_object(object)){
		return false;
	}

	string table_name = object.get_tableName();
	this->table_name = table_name;

	string sql = "select * from " + table_name;
	sql += " where " + get_limits(object) +  " order by create_time desc limit 10;";

	string log = "time: " + get_time();
	log += "\t" + sql;
	write(1, log.data(), log.length());
	return database->query(sql.data());

}

bool TableDao::queryPageData(const string & table_name, const int & page_num, const string & limit_key, const string & limit_value){

	this->table_name = table_name;
	string sql = "select * from " + table_name;
	if(limit_key.compare("") != 0){

		sql += " where " + limit_key + " = " + "'" + limit_value + "'";
	}
	sql += " order by create_time desc limit " + to_string( (page_num - 1) * 10 ) + " , 10;";

	string log = "time: " + get_time();
	log += "\t" + sql;
	write(1, log.data(), log.length());
	return database->query(sql.data());
}

bool TableDao::insertData(const VoTable & object){

	if(!judge_object(object)){
		return false;
	}

	vector<string> fields = object.get_fields();
	vector<string> values = object.get_values();
	string table_name = object.get_tableName();
	this->table_name = table_name;
	 
	int len = fields.size();
	string insert_fields = "";
	string insert_values = "";


	for(int i = 0; i < len; ++i){

		if(!values[i].empty()){
			
			bool flag = !insert_fields.empty();
			insert_fields += (flag ? ", " : "");
			insert_fields += fields[i];
			insert_values += (flag ? ", " : "");
			insert_values += "'" + values[i] + "'";
		}
	}

	string sql = "insert into " + table_name;
	sql += "(" + insert_fields + ")";
	sql += " values(" + insert_values + ");";

	string log = "time: " + get_time();
	log += "\t" + sql;
	write(1, log.data(), log.length());

	return database->insert(sql.data());
}

bool TableDao::deleteData(const VoTable & object){

	if(!judge_object(object)){
		return false;
	}

	this->table_name = object.get_tableName();
	string sql = "delete from " + object.get_tableName();
	sql += " where " + get_limits(object) + ";";
	
	string log = "time: " + get_time();
	log += "\t" + sql;
	write(1, log.data(), log.length());

	return database->del(sql.data());
}

bool TableDao::updateData(const VoTable & old_object, const VoTable & new_object){

	if(!judge_object(old_object) || !judge_object(new_object)){
		return false;
	}

	this->table_name = new_object.get_tableName();
	string sql = "update " + new_object.get_tableName();
	sql += " set " + get_limits(new_object, false);
	sql += " where " + get_limits(old_object) + ";";

	string log = "time: " + get_time();
	log += "\t" + sql;
	write(1, log.data(), log.length());

	return database->update(sql.data());
}

bool TableDao::set_log(const string & log_info, const int & manager_id, const string & manager_ip){

	try{
		string sql = "insert into manage_log(manager_id, manager_ip, action_content) values(";
		sql += to_string(manager_id) + ", '" + manager_ip + "', '" + log_info + "');";

		return database->insert(sql.data());

	}catch(...){

		return false;
	}
}

vector<VoTable> TableDao::get_objects(){

	objects.clear();

	string ** result = database->get_result();

	if(result == NULL){
		return objects;
	}

	int row_num = database->get_rowNum();
	int col_num = database->get_colNum();

	VoTable object;
	object.set_tableName(this->table_name);

	vector<string> values;

	for(int i = 0; i < row_num; ++i){

		values.clear();
		for(int j = 0; j < col_num; ++j){
				
			values.push_back(result[i][j]);	
		}

		object.set_values(values);
		this->objects.push_back(object);
	}

	database->free_result();

	return objects;

}










