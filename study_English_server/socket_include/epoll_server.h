#ifndef __EPOLLSERVER_H__
#define __EPOLLSERVER_H__

#include <sys/epoll.h>

#include "select_server.h"
#include "event_control.h"
#include "record_event.h"


class EpollServer : public SelectServer{

public:
	EpollServer();
	~EpollServer();

	void check_active();

	virtual void init_data();
	virtual void process_run();
	virtual void wait_requests();
	virtual void accept_clients();
	virtual void close_client(const int & client_fd);
	virtual void deal_clients_messages();


private:
	int epoll_fd;

	struct epoll_event * socket_events;	
	RecordEvent * record_events;
	EventControl * event_control;

};


#endif
	
