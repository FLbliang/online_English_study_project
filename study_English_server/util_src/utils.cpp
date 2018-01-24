#include <stdlib.h>
#include <time.h>
#include <string>

#include "utils.h"

using std::string;
using std::to_string;

string get_time(){

	struct tm * c_time;
	char buf[666];
	time_t t;
	time(&t);
	c_time = localtime(&t);
	strftime(buf, sizeof(buf), "%F %T", c_time);

	return string(buf);
}

bool is_int(const string & str){

	int len = str.length();
	for(int i = 0; i < len; ++i){
		
		if(str[i] < '0' || str[i] > '9'){
			return false;
		}
	}
	return true;

}

bool is_float(const string & str){

	int len = str.length();
	bool dot_flag = false;

	for(int i = 0; i < len; ++i){
		
		if(str[i] == '.' && !dot_flag){
			dot_flag = true;
			continue;
		}

		if(str[i] < '0' || str[i] > '9'){
			return false;
		}
	}

	return true;

}

string get_connectCode(){

	srand((unsigned)time(0));
	unsigned int num = rand();
	return to_string(num);

}
	




















