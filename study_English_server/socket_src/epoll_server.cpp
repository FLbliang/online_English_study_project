#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>

#include "utils.h"
#include "epoll_server.h"


using std::string;

const int client_bufsize = 8666;

EpollServer::EpollServer(){

	epoll_fd = 0;
	socket_events = NULL;
	record_events = NULL;
	event_control = NULL;

	string buf = "time: ";
	buf += get_time();
	buf += "\tepoll server is starting ......\n";
	
	net_api.Write(1, buf.data(), buf.length());
}

EpollServer::~EpollServer(){

	if(init_flag){
		for(int i = 0; i < client_max_num; ++i){
			
			if(record_events[i].get_fd() != -1){
				net_api.Close(record_events[i].get_fd());
			}
		}
		delete [] client_addrs;
		delete [] socket_events;
		delete [] record_events;
		delete event_control;
	}

}

void EpollServer::init_data(){

	if( !client_max_num ) return;

	epoll_fd = epoll_create(client_max_num);

	if(epoll_fd == -1){
		net_api.Perror("epoll_create error");
	}
	client_addrs = new sockaddr_in [client_max_num];
	socket_events = new epoll_event [client_max_num];
	record_events = new RecordEvent [client_max_num];
	event_control = new EventControl;	

	event_control->set_epollfd(epoll_fd);
	if(fcntl(listen_fd, F_SETFL, O_NONBLOCK) < 0){
		net_api.Perror("fcntl listen_fd error");
	}
	socket_events[0].events = EPOLLIN;
	socket_events[0].data.fd = listen_fd;

	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &socket_events[0]) < 0){
			net_api.Perror("epoll_ctl add listen_fd error");
	}

	init_flag = true;

	
}

void EpollServer::process_run(){

	if( !init_flag )return;
	
	int check_num = 0;

	while(true){

		wait_requests();
		
		if(check_num >= 100){

			check_active();
						
			check_num = 0;
		}

		if(request_nums > 0){

			deal_clients_messages();
		}

		++check_num;
	}
		
}

void EpollServer::check_active(){

	int current_time = time(NULL);
	for(int i = 0; i < client_max_num; ++i){
		if(record_events[i].get_fd() == -1 || !record_events[i].get_status())
			continue;

		if(current_time - record_events[i].get_active() >= 6000){
			close_client(record_events[i].get_fd());
		}
		
	}

}

void EpollServer::wait_requests(){
	
	request_nums = epoll_wait(epoll_fd, socket_events, client_max_num, 600);	

}

void EpollServer::accept_clients(){

	string buf;
	int coord = -1;
	for(int i = 0; i < client_max_num; ++i){
		
		if(record_events[i].get_fd() == -1){
			coord = i;
			break;
		}
	}
	if(coord == -1){
		
		buf = "time: ";
		buf += get_time();
		buf += "\tclient number out of bound!\n";
		net_api.Write(1, buf.data(), buf.length());
		return;
	}

	socket_len = sizeof(client_addrs[coord]);
	int client_fd = net_api.Accept(listen_fd, (struct sockaddr *) & client_addrs[coord], & socket_len);
	
	if(fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0){

		buf = "time: ";
		buf += get_time();
		buf += "\tfcntl nonblocking client_fd failed!\n";
		net_api.Write(1, buf.data(), buf.length());
		return;
	}

	event_control->event_set(client_fd, &record_events[coord]);
	event_control->event_add(EPOLLIN, &record_events[coord]);

	if(record_events[coord].get_bufSize() <= 0){
		record_events[coord].set_bufSize(client_bufsize);
	}

	map_coord[client_fd] = coord;

	buf = "time: ";
	buf += get_time();
	buf += "\tA client connect.............\n";
	net_api.Write(1, buf.data(), buf.length());

	char bufdata[BUFSIZ];
	const char * ip = inet_ntop(AF_INET, &client_addrs[coord].sin_addr, bufdata, sizeof(bufdata));
	record_events[coord].set_ip(ip);
	display_socket_addr(coord);
	
}

void EpollServer::close_client(const int & client_fd){

	string buf = "time: ";
	buf += get_time();
	buf += "\tA client disconnect................\n";
	
	int coord = map_coord[client_fd];
	record_events[coord].set_fd(-1);
	
	net_api.Write(1, buf.data(), buf.length());
	display_socket_addr(coord);
	net_api.Close(client_fd);
	
}

void EpollServer::deal_clients_messages(){

	for(int i = 0; i < request_nums; ++i){

		if(socket_events[i].data.fd == listen_fd && (socket_events[i].events & EPOLLIN)){
				
			accept_clients();
			continue;
		}

		RecordEvent * re_ev = (RecordEvent *) socket_events[i].data.ptr;

		if((socket_events[i].events & EPOLLIN) && (re_ev->get_events() & EPOLLIN)){
			
			if(re_ev->recv_data(event_control) == false){
				close_client(re_ev->get_fd());
			}else{
				re_ev->set_active(time(NULL));
			}
		}
			
		if((socket_events[i].events & EPOLLOUT) && (re_ev->get_events() & EPOLLOUT)){

			if(re_ev->send_data(event_control) == false){
				close_client(re_ev->get_fd());
			}else{
				re_ev->set_active(time(NULL));
			}
		}
			
	}

}


















