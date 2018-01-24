#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <sys/epoll.h>

#include "event_control.h"
#include "bo_controller.h"
#include "record_event.h"

using std::string;
using namespace std;


RecordEvent::RecordEvent(){

	this->bo_controller = new BoController();
	buf_size = 0;
	fd = -1;
	events = -1;
	status = 0;
	buf = NULL;
	len = 0;
	active = time(NULL);

}

RecordEvent::~RecordEvent(){
	delete [] buf;
	delete bo_controller;
}

void RecordEvent::set_ip(const string & ip){
	bo_controller->set_managerIp(ip);
}

bool RecordEvent::send_data(EventControl * event_control){

	if(send_buf.length() <= 0)return true;
	
	ssize_t flag = send(fd, send_buf.data(), send_buf.length(), 0);
	
	if(flag > 0){
			
		len = 0;
		send_buf.clear();
		event_control->event_add(EPOLLIN, this);
		return true;
		
	}else{
		return false;
	}

}

bool RecordEvent::recv_data(EventControl * event_control){

	len = recv(fd, buf, buf_size, 0);
	
	int bo_status = 0;

	if(len > 0){
		string data = buf;

		do{

			len = recv(fd, buf, buf_size, 0);
			if(len > 0){
				data += buf;
			}

		}while(len > 0);

		bo_status = (bo_controller->explain(data) ? 1 : 0 );

		send_buf = bo_controller->get_sendBuf(bo_status);

		event_control->event_add(EPOLLOUT, this);

		return true;
		
	}else{
		return false;
	}
}




















