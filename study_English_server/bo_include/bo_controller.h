#ifndef __BOCONTROLLER__
#define __BOCONTROLLER__

#include <string> 
#include <boost/property_tree/ptree.hpp>

#include "vo_table.h"
#include "table_dao.h"

using std::string;
using namespace boost::property_tree;


class BoController{

public:

	BoController();
	~BoController();

	bool explain(const string & data);

	void set_connectCode(const string & code){ this->connect_code = connect_code; }
	string get_code(){ return connect_code; }
	void set_managerIp(const string & manager_ip){ this->manager_ip = manager_ip; }
	bool judge_connectCode(const string & connect_code);
	string get_logInfo(const string & order, const VoTable * table, const VoTable * old_table = NULL);

	// 辅助方法
	VoTable get_requestTable(const string & table_name, const ptree & pt, bool & get_flag);

	VoTable get_requestLoginTable(const ptree & pt, bool & get_flag);
	VoTable get_requestSearchTable(const ptree & pt, bool & get_flag);
	VoTable get_requestInsertTable(const ptree & pt, bool & get_flag);
	vector<VoTable> get_requestUpdateTable(const ptree & pt, bool & get_flag);
	vector<VoTable> get_requestDeleteTable(const ptree & pt, bool & get_flag);

	string get_sendBuf(int status = 0);

	//void test_show(const vector<string> & strs);


private:
	TableDao table_dao;
	int manager_id;
	string manager_ip;
	int result_sum;
	string connect_code;
	string response_header;
	bool get_result;
	

};

#endif
