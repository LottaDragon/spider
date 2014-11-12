/**
 * file: mocker_pp.cpp
 * nothing to do in the mocker_pp,
 * just receive the data and throw away
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ul_log.h"
#include "ul_net.h"
#include "nshead.h"
#include "mc_pack.h"

//#include "../public/vs-spider.h"
#include "vs-spider.h"

#include "init.h"
#include "util.h"

#define MAX_LINSTEN_NUM 	100


//连接管理元信息
struct conninfo{
	int handle[MAX_LINSTEN_NUM];	//存储fd的数组
	int fd_pos;						//下一轮fd偏移最大位置
	int maxfd;						//最大的句柄
	fd_set readfds;					//下一轮的readfds
};

static conninfo g_conn_mgr;

// 初始化连接的元信息
void init_conn_mgr(int listen_sock)
{
	g_conn_mgr.handle[0] = listen_sock;	
	for(int i = 1 ; i < MAX_LINSTEN_NUM ; i++ ){
		g_conn_mgr.handle[i] = -1;
	}
	g_conn_mgr.fd_pos = 0;
	FD_SET(listen_sock, &(g_conn_mgr.readfds) );
	g_conn_mgr.maxfd = listen_sock;
}

// 关闭某个连接
void close_socket(int offset)
{
	int sock = g_conn_mgr.handle[offset];
	FD_CLR( sock, &(g_conn_mgr.readfds) );
	close( sock );
	g_conn_mgr.handle[offset] = -1;	
	_LOG_NOTICE("removing sock on [pos:%d][fd:%d]", offset, sock);     
}

// 添加新的连接
int do_recv_conn(int new_sock)
{
	//find the new_pos offset
	int new_pos = 1;
	for( ; new_pos < MAX_LINSTEN_NUM ; new_pos++){
		if( g_conn_mgr.handle[new_pos] == -1){
			g_conn_mgr.handle[new_pos] = new_sock;	
			break;
		}
	}

	/**
	 * crawler receiver keeps connection with the peer,
	 * so the connnection num is limited
	 */
	if( new_pos >= MAX_LINSTEN_NUM){
		_LOG_WARNING("too many sockets and close new one:[fd:%d]",new_sock);
		close(new_sock);
		return -1;
	}

	//update the max pos fd_pos
	if( new_pos > g_conn_mgr.fd_pos){
		g_conn_mgr.fd_pos = new_pos;
	}

	FD_SET(new_sock, &(g_conn_mgr.readfds));

	if( new_sock > g_conn_mgr.maxfd ){
		g_conn_mgr.maxfd = new_sock;
	}	
	
	_LOG_NOTICE("adding sock on [pos:%d][fd:%d]", new_pos,new_sock);
	return 0;
}

// 8MB buffer for nshead read
#define _M_ (1<<23)
char read_buf[_M_];
char tmpbuf[1280*1024];
char _tmpbuf[10240];

int do_recv_data(int handle)
{
	//read crawl packet
	nshead_t *nh = (nshead_t *)read_buf;


	int ret = nshead_read( g_conn_mgr.handle[ handle ], nh, sizeof(read_buf), 5000);
	if( ret < 0 ){
		_LOG_WARNING("read failed! %s. %m",nshead_error(ret));
		//read fail and close the socket
		close_socket(handle);
		return -1;
	}

//	if(!mc_pack_valid(p, nh->body_len)){
//		_LOG_WARNING("not a valid pack!");
//		close_socket(handle);
//		return -1;
//	}

	char *buf = (char*)(nh+1);
	int buflen = sizeof(read_buf)-sizeof(nshead_t);

	mc_pack_t *p = mc_pack_open_r( buf, buflen, _tmpbuf, sizeof(_tmpbuf));

	if( MC_PACK_PTR_ERR(p) ){     
		_LOG_WARNING("open mcpack failed");
		return -1;
	}  

	*tmpbuf = '\0';
	/*
	_LOG_TRACE("%s",tmpbuf);
	*/
	mc_pack_pack2text(p, tmpbuf, sizeof(tmpbuf), 0 );
	printf("***************************************\n");
	printf("%s\n",tmpbuf);
	printf("***************************************\n");


	close_socket(handle);

	_LOG_NOTICE("Get a pack and handle it");

	// simulation for pack handler process, like ec
	Nanosleep(5);

	return 0;
}

void do_receive()
{

	while(1){
		int fd_pos = g_conn_mgr.fd_pos;
		fd_set readfds = g_conn_mgr.readfds;
		int maxfd = g_conn_mgr.maxfd;	

		struct timeval tv;
		//every 4 seconds
		tv.tv_sec = 4;
		tv.tv_usec = 0;

		int ready = select(maxfd+1, &readfds, 0, 0, &tv);
		if( ready < 0 ){
			_LOG_WARNING("crawler select error! %m");	
			continue;
		}
		if( ready == 0){
			_LOG_WARNING("crawler select timeout in 4 seconds");	
			continue;
		}


		for(int i = 0; i <= fd_pos; i++){
			if( g_conn_mgr.handle[i] < 0 ){
				continue;
			}
			if( FD_ISSET( g_conn_mgr.handle[i], &readfds) ){
				//accept new connection
				if( i == 0 ){
					int sock = g_conn_mgr.handle[0];
					struct sockaddr_in addr;
					socklen_t addrlen = sizeof(struct sockaddr_in);
					int new_sock = accept(sock, (struct sockaddr *)&addr, &addrlen);
					while( new_sock == -1 && errno == EINTR){
						_LOG_WARNING("accept failed, %m");
						new_sock = accept(sock, (struct sockaddr *)&addr, &addrlen);
					}
					if( new_sock == -1){
						_LOG_WARNING("accept failed, %m");
					}else{
						do_recv_conn(new_sock);
					}	

				} else{
					do_recv_data(i);
				}	
				
				if( --ready <= 0 ){
					break;
				}
			}	
		}
		
	}
}

int main(int argc, char **argv)
{
	if(argc != 2){
		const char* usage = "./mocker_pp $which_port_to_bind\n";
		puts(usage);
		return -1;
	}

	int port = atoi(argv[1]);

	ul_logstat_t logstat;

	logstat.events = 0x10; //debug
	logstat.to_syslog = 0; //do not write to syslog
	logstat.spec = 0x0;

	ul_openlog(".", "mocker_pp.", &logstat, 2048);

	signal(SIGPIPE, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

	int sock = ul_tcplisten(port, 5);
	if ( sock < 0 ) {
		_LOG_FATAL("ul_tcplisten failed, %m");
		return -1;
	}

	_LOG_NOTICE("PP listen successfully ...");

	init_conn_mgr(sock);

	do_receive();

	assert(0);

	return 0;
}
