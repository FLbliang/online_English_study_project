#ifndef __DATABASE__
#define	__DATABASE__

#include <mysql.h>
#include <string>

using std::string;

class DataBase{

private:
	const char * user;
	const char * password;
	const char * host;
	const char * database;
	MYSQL * mysql;
	string ** result;
	int row_num;
	int col_num;

public:
	DataBase(const char * user, const char * password, const char * host, const char * database);
	~DataBase();

	inline string ** get_result(){ return result; }
	inline int get_rowNum() { return row_num; }
	inline int get_colNum() { return col_num; }

	void free_result();
	bool connect();
	void disconnect();
	bool query(const char * sql, bool is_query = true);
	bool del(const char * sql);
	bool insert(const char * sql);
	bool update(const char * sql);
	bool set_result();
};

#endif
