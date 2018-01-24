#include <stdlib.h>
#include <unistd.h>
#include <exception>
#include <stdio.h>
#include <string>

#include "utils.h"
#include "database.h"

using std::string;
using namespace std;

DataBase::DataBase(const char * user, const char * password, const char * host, const char * database){
	this->user = user;
	this->password = password;
	this->host = host;
	this->database = database;
} 

DataBase::~DataBase(){

	free_result();
}

bool DataBase::connect(){

	mysql = mysql_init(NULL);
	if(mysql_real_connect(mysql, host, user, password, database, 3306, NULL, 0)){
		return true;
	}else{
		return false;
	}
}

void DataBase::disconnect(){

	mysql_close(mysql);

}

bool DataBase::query(const char * sql, bool is_query){

	if(!this->connect()){
		return false;
	}

	if(mysql_set_character_set(mysql, "utf8")){
		return false;
	}

	bool flag = (mysql_autocommit(mysql, 0) == 0)
				&&
				(mysql_query(mysql, sql) == 0)
				&&
				(mysql_commit(mysql) == 0);

	if(is_query){
		flag = set_result();
	}
	
	this->disconnect();
	return flag;
	
}

bool DataBase::del(const char * sql){
	return this->query(sql, false);
}

bool DataBase::insert(const char * sql){
	return this->query(sql, false);
}

bool DataBase::update(const char * sql){
	return this->query(sql, false);
}

bool DataBase::set_result(){

	this->result = NULL;
	MYSQL_RES * result_set = mysql_store_result(mysql);
	if(result_set == NULL){
		return false;
	
	}

	MYSQL_ROW row;

	row_num = mysql_num_rows(result_set);
	col_num = mysql_num_fields(result_set);


	int err_coord = -1;
	this->result = new string * [row_num];
	for(int i = 0; i < row_num; ++i){
		this->result[i] = new string [col_num];
		if(!(row = mysql_fetch_row(result_set)) ){
			err_coord = i;
			break;
		}


		for(int j = 0; j < col_num; ++j){
			
			this->result[i][j] = row[j] ? row[j] : "";
		}


	}	


	if(err_coord >= 0){

		for(int i = 0; i <= err_coord; ++i){
			delete [] this->result[i];
		}
		
		delete [] result;
		this->result = NULL;
		mysql_free_result(result_set);
		return false;
	}
	
	mysql_free_result(result_set);


	return true;
	
}

void DataBase::free_result(){

	if( result == NULL ) return;

	for(int i = 0; i < row_num; ++i){
		delete [] result[i];
	}

	delete [] result;
	result = NULL;
}

