#ifndef __RECORDEVENT_H__
#define __RECORDEVENT_H__

#include <string>

#include "bo_controller.h"

using std::string;

class EventControl;

class RecordEvent{

public:
	RecordEvent();
	~RecordEvent();

	void set_bufSize(const int & buf_size){ this->buf_size = buf_size; buf = new char [buf_size]; }
	int get_bufSize() const { return buf_size; }
	void set_fd(const int & fd){ this->fd = fd; }
	int get_fd() const { return fd; } 
	void set_events(const int & events){ this->events = events; }
	int get_events() const { return events; }
	void set_status(const int & status){ this->status = status; }
	int get_status() const { return status; }
	void set_active(const long long & active){ this->active = active; }
	long long get_active() const { return active; }
	void set_ip(const string & ip);
	
	bool send_data(EventControl * event_control);
	bool recv_data(EventControl * event_control);

	
private:
	int buf_size;
	int fd;
	int events;
	int status;
	char * buf;
	string send_buf;
	int len;
	long long active;

	BoController * bo_controller;
	
	
};


#endif 
