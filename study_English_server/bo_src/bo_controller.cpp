#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <vector>

#include "utils.h"
#include "vo_table.h"
#include "bo_controller.h"
#include "table_dao.h"

using namespace boost::algorithm;
using namespace boost::property_tree;
using namespace std;

BoController::BoController(){
	
	connect_code = "";
}

BoController::~BoController(){

}

bool BoController::explain(const string & data){

	this->get_result = false;

	try{

		stringstream stream(data);
		ptree pt;
		read_json(stream, pt);

		string order = pt.get<string>("order");
		this->response_header = order + "_response";

		bool get_flag = true;
		VoTable request_table;

		if(order.compare("request_login") == 0){

			request_table = this->get_requestLoginTable(pt, get_flag);

			if(get_flag){
				this->connect_code = get_connectCode();
			}

			return get_flag
					&&
					table_dao.querySearchData(request_table);

		}else if(order.compare("request_pageData") == 0){

			if(!judge_connectCode(pt.get<string>("code"))){
				return false;
			}
		
			string table_name = pt.get<string>("table_name");
			string page_num = pt.get<string>("page_num");
			string key, value;
			try{
				key = pt.get<string>("key");
				value = pt.get<string>("value");

			}catch(...){
				key = "";
				value = "";
			}

			if(!is_int(page_num)){
				return false;
			}

			this->get_result = true;
			return 	table_dao.get_tableSum(result_sum, table_name, key, value)
					&&
					table_dao.queryPageData(table_name, stoi(page_num), key, value);

		}else if(order.compare("request_searchData") == 0){

			if(!judge_connectCode(pt.get<string>("code"))){
				return false;
			}

			request_table = this->get_requestSearchTable(pt, get_flag);

			string key = request_table.get_fields()[0];
			string value = request_table.get_values()[0];

			this->get_result = true;
			return get_flag
					&&
					table_dao.get_tableSum(result_sum, request_table.get_tableName(), key, value)
					&&
					table_dao.querySearchData(request_table);

		}else if(order.compare("request_delete") == 0){

			if(!judge_connectCode(pt.get<string>("code"))){
				return false;
			}

			vector<VoTable> request_tables = this->get_requestDeleteTable(pt, get_flag);

			if(!get_flag){
				return false;
			}

			vector<VoTable>::iterator tables_pointer = request_tables.begin();

			for(; tables_pointer != request_tables.end(); ++tables_pointer){

				if(!table_dao.deleteData(*tables_pointer)){
					return false;
				}else{
					string log_info = get_logInfo(order, &*tables_pointer);
					if(!table_dao.set_log(log_info, manager_id, manager_ip)){
						return false;
					}
				}
						
			}

			return true;


		}else if(order.compare("request_update") == 0){

			if(!judge_connectCode(pt.get<string>("code"))){
				return false;
			}

			vector<VoTable> request_tables;

			request_tables = this->get_requestUpdateTable(pt, get_flag);

			return	get_flag
					&&
					request_tables.size() == 2
					&&
					table_dao.updateData(
								*(request_tables.begin()),
								*(request_tables.begin() + 1)
					)
					&&
					table_dao.set_log(
								get_logInfo(
										order, 
										&*(request_tables.begin() + 1), 
										&*(request_tables.begin())
								),
								manager_id,
								manager_ip
					);
				

		}else if(order.compare("request_insert") == 0){

			if(!judge_connectCode(pt.get<string>("code"))){
				return false;
			}

			request_table = this->get_requestInsertTable(pt, get_flag);

			return get_flag
					&&
					table_dao.insertData(request_table)
					&&
					table_dao.set_log(
								get_logInfo(order, &request_table),
								manager_id,
								manager_ip
					);

		}else{

			return false;
		}

	}catch(...){

		return false;

	}

}

string BoController::get_logInfo(const string & order, const VoTable * table, const VoTable * old_table){

	string log_info;
	string table_name = table->get_tableName();
	vector<string> fields = table->get_fields();
	vector<string> values = table->get_values();
	

	if(order.compare("request_insert") == 0){
		log_info = "insert ";

	}else if(order.compare("request_delete") == 0){
		log_info = "delete ";

	}else if(order.compare("request_update") == 0){
		log_info = "update ";
	}

	log_info += table_name + " for ";
	unsigned int len = values.size();
	len = fields.size() == len ? len : 0;

	if(old_table == NULL && len){
		for(unsigned int i = 0; i < len; ++i){
			int str_len = fields[i].length();
			string data_id, create_time;
			if(str_len < 2){
				continue;	
			}else {
				data_id = fields[i].substr(str_len-2, 2);
				
			}

			if(str_len >= 4){
				create_time = fields[i].substr(str_len-4, 4);
			}

			if(data_id.compare("id") == 0 || create_time.compare("time") == 0){
				log_info += fields[i] + " = " +  values[i] + " ";
			}else if(fields[i].compare("username") == 0){
				log_info += fields[i] + " = " + values[i] + " ";
			}
		}

	}else if(len){
		vector<string> old_values = old_table->get_values();
		len = old_values.size() == len ? len : 0;

		for(unsigned int i = 0; i < len; ++i){
			int str_len = fields[i].length();
			string data_id, create_time;
			if(str_len < 2){
				continue;	
			}else {
				data_id = fields[i].substr(str_len-2, 2);
				
			}

			if(str_len >= 4){
				create_time = fields[i].substr(str_len-4, 4);
			}

			if(data_id.compare("id") == 0 || create_time.compare("time") == 0){
				log_info += fields[i] + " = " +  old_values[i] + " ";
			}else if(fields[i].compare("username") == 0){
				log_info += fields[i] + " = " + old_values[i] + " ";
			}
		}

		log_info += "update fields for ";
		for(unsigned int i = 0; i < len; ++i){
			if(old_values[i].compare(values[i]) != 0){
				log_info += fields[i] + " ";
			}
		}
	}

	return log_info;
}

bool BoController::judge_connectCode(const string & str){

	return str.compare(this->connect_code) == 0;
}

VoTable BoController::get_requestTable(const string & table_name, const ptree & pt, bool & get_flag){

	VoTable table;
	try{

		vector<string> fields;
		vector<string> values;

		table.set_tableName(table_name);

		BOOST_FOREACH(const ptree::value_type & v, pt){

			fields.push_back(v.first);
			values.push_back(v.second.data());
		}

		table.set_fields(fields);
		table.set_values(values);

	}catch(...){

		get_flag = false;

	}

	return table;	
}

VoTable BoController::get_requestLoginTable(const ptree & pt, bool & get_flag){

	VoTable table;
	try{
		vector<string> fields;
		vector<string> values;
		fields.push_back("username");
		fields.push_back("password");
		values.push_back(pt.get<string>("username"));
		values.push_back(pt.get<string>("password"));

		table.set_fields(fields);
		table.set_values(values);
		table.set_tableName("manager_info");

	}catch(...){

		get_flag = false;

	}

	return table;

}

VoTable BoController::get_requestSearchTable(const ptree & pt, bool & get_flag){
	
	VoTable table;
	try{
		vector<string> fields;
		vector<string> values;

		fields.push_back(pt.get<string>("key"));
		values.push_back(pt.get<string>("value"));

		table.set_fields(fields);
		table.set_values(values);
		table.set_tableName(pt.get<string>("table_name"));

	}catch(...){

		get_flag = false;
	}

	return table;
}

VoTable BoController::get_requestInsertTable(const ptree & pt, bool & get_flag){

	VoTable table;
	try{

		table = get_requestTable(pt.get<string>("table_name"), pt.get_child("data"), get_flag);

	}catch(...){
		get_flag = false;
	}

	return table;
}

vector<VoTable> BoController::get_requestUpdateTable(const ptree & pt, bool & get_flag){

	vector<VoTable> tables;

	try{

		string table_name = pt.get<string>("table_name");
		VoTable table;
		
		table = get_requestTable(table_name, pt.get_child("old_data"), get_flag);	

		if(!get_flag){
			return tables;
		}

		tables.push_back(table);
		table = get_requestTable(table_name, pt.get_child("new_data"), get_flag);

		if(!get_flag){
			return tables;
		}

		tables.push_back(table);

	}catch(...){

		get_flag = false;
	}
	
	return tables;

}

vector<VoTable>  BoController::get_requestDeleteTable(const ptree & pt, bool & get_flag){

	vector<VoTable> tables;

	try{

		string table_name = pt.get<string>("table_name");
		VoTable table;
		
		ptree data = pt.get_child("data");

		BOOST_FOREACH(ptree::value_type & v, data){

			table = get_requestTable(table_name, v.second, get_flag);

			if(get_flag == false){
				return tables;
			}

			tables.push_back(table);
			
		}

	}catch(...){
		get_flag = false;
	}


	return tables;
	

}

string BoController::get_sendBuf(int status){
		
	try{
		stringstream stream;
		ptree pt;
		ptree data;
		
		pt.put("response", this->response_header);
		
		if(response_header.compare("request_login_response") == 0 && status){
			vector<VoTable> result_tables = table_dao.get_objects();
			if(result_tables.size() != 0){

				pt.put("code", connect_code);
				vector<VoTable>::iterator table_pointer = result_tables.begin();
				this->manager_id = stoi(table_pointer->get_values()[0]);

				table_dao.set_log("login", manager_id, manager_ip);
				table_dao.set_loginTime(manager_id);
				
			}else{
				status = 0;
			}
		}

		if(get_result){

			vector<VoTable> result_tables = table_dao.get_objects();
			if(result_tables.size() == 0){
				pt.put("status", to_string(0));

			}else{
				pt.put("status", to_string(status));
				pt.put("result_sum", this->result_sum);

				vector<VoTable>::iterator tables_pointer;
				vector<string> values;

				for(tables_pointer = result_tables.begin();
					tables_pointer != result_tables.end();
					++tables_pointer){

					values = tables_pointer->get_values();
			
					ptree item;
					
					int len = values.size();

					for(int i = 0; i < len; ++i){
						ptree value;
						value.put("", values[i]);
						item.push_back(make_pair("", value));
					}

					data.push_back(make_pair("", item));
				
				}

				pt.push_back(make_pair("data", data));
				pt.put("values_sum", to_string(this->result_sum));
			}

		}else{
			pt.put("status", to_string(status));
				
		}
			
		write_json(stream, pt);

		return stream.str();

	}catch(...){

		return "error";
	}
	
}

























