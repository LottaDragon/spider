/**
 * file: nullcrawler.cpp
 * nothing to do in the nullcrawler,
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

#include "receiver.h"
#include "init.h"

#define MAX_LINSTEN_NUM 	100


//连接管理元信息
struct conninfo{
	int handle[MAX_LINSTEN_NUM];	//存储fd的数组
	int fd_pos;						//下一轮fd偏移最大位置
	int maxfd;						//最大的句柄
	fd_set readfds;					//下一轮的readfds
};

static conninfo g_conn_mgr;
static char tmpbuf[10240];

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

int do_recv_data(int handle)
{
	//buffer for nshead read
	char read_buf[64*1024];

	//read crawl packet
	nshead_t *nh = (nshead_t *)read_buf;

	int ret = nshead_read( g_conn_mgr.handle[ handle ], nh, sizeof(read_buf), 1000);
	if( ret < 0 ){
		_LOG_WARNING("read failed! %s. %m",nshead_error(ret));
		//read fail and close the socket
		close_socket(handle);
		return -1;
	}

	char *buf = (char*)(nh+1);
	int buflen = sizeof(read_buf)-sizeof(nshead_t);

	mc_pack_t *p = mc_pack_open_r( buf, buflen, tmpbuf, sizeof(tmpbuf));

	if( MC_PACK_PTR_ERR(p) ){     
		_LOG_WARNING("open mcpack failed");
		return -1;
	}  

	uint32_t packtype;

	if( mc_pack_get_uint32(p,"Pack_type",&packtype) != 0 ){
		_LOG_WARNING("get \"packtype\" error!");							
		close_socket(handle);
		return -1;
	}

	/**
	 * when mcpack update to 2.0 and could modify the value of key,
	 * we change the packtype
	 */
	if( packtype != /*PACK_LINKDB_SELECT*/ PACK_CC2CR ){
		_LOG_WARNING("packtype do not match [%u]", packtype);	
		close_socket(handle);
		return -1;
	}

	// malloc in receiver thread and free in the handle thread
	req_crawl_t *req = (req_crawl_t *)malloc(sizeof(req_crawl_t));
	assert(req);

	uint32_t ntmp;
	const char*	stmp;

	stmp = mc_pack_get_str(p,"url");
	if( MC_PACK_PTR_ERR(stmp) == 0 ){
		strncpy(req->url,stmp,MAX_URL_LEN);
		req->url[MAX_URL_LEN-1] = 0;
	}else{
		_LOG_WARNING("can not get url");
		close_socket(handle);
		free(req);
		return -1;
	}

	stmp = mc_pack_get_str(p,"pp_ip");
	if( MC_PACK_PTR_ERR(stmp) == 0 ){
		strncpy(req->pp_ip,stmp,32);
		req->pp_ip[31] = 0;
	}else{
		_LOG_WARNING("can not get pp_ip");
		close_socket(handle);
		free(req);
		return -1;
	}

	if( mc_pack_get_int32(p,"pp_port",&(req->pp_port)) != 0){
		_LOG_WARNING("can not get pp_port");
		close_socket(handle);
		free(req);
		return -1;
	}

	if( mc_pack_get_uint32(p,"is_cookie",&ntmp) == 0 ){
		if( ntmp ==1 ){
			stmp = mc_pack_get_str(p,"cookie");
			if( MC_PACK_PTR_ERR(stmp) == 0 ){
				strncpy(req->cookie,stmp,MAX_COOKIE_LEN);
				req->cookie[MAX_COOKIE_LEN-1] = 0;
			}else{
				_LOG_WARNING("set cookie but can not get cookie");
				close_socket(handle);
				free(req);
				return -1;
			}
		}else{
			req->cookie[0] = 0;
		}
	}else{
		_LOG_WARNING("can not get is_cookie");
		req->cookie[0] = 0;
	}

	if( mc_pack_get_uint32(p,"is_referrer",&ntmp) == 0 ){
		if( ntmp ==1 ){
			stmp = mc_pack_get_str(p,"referrer");
			if( MC_PACK_PTR_ERR(stmp) == 0 ){
				strncpy(req->referrer,stmp,MAX_REFERRER_LEN);
				req->referrer[MAX_REFERRER_LEN-1] = 0;
			}else{
				_LOG_WARNING("set referrer but can not get referrer");
				close_socket(handle);
				free(req);
				return -1;
			}
		}else{
			req->referrer[0] = 0;
		}
	}else{
		_LOG_WARNING("can not get is_referrer");
		req->referrer[0] = 0;
	}

	stmp = mc_pack_get_str(p,"s_ipaddr");
	if( MC_PACK_PTR_ERR(stmp) == 0 ){
		strncpy(req->ip,stmp,32);
		req->ip[31] = 0;
	}else{
		req->ip[0] = 0;
	}

	if( mc_pack_get_int32(p,"s_port",&(req->port)) != 0){
		req->port = 0;
	}

	stmp = mc_pack_get_str(p,"method");
	if( MC_PACK_PTR_ERR(stmp) == 0 ){
		strncpy(req->method,stmp,64);
		req->method[63] = 0;
	}else{
		strcpy(req->method,"GET");
	}

	stmp = mc_pack_get_str(p,"http_version");
	if( MC_PACK_PTR_ERR(stmp) == 0 ){
		strncpy(req->protocol,stmp,64);
		req->protocol[63] = 0;
	}else{
		strcpy(req->protocol,"HTTP/1.0");
	}

	ret = mc_pack_pack2text(p,req->req_buff,sizeof(req->req_buff),0);	
	if( ret !=0){
		_LOG_WARNING(" mc_pack_pack2text error");	   
		close_socket(handle);
		free(req);
		return -1;
	}

	
	char logbuf[8192];
	req->to_string(logbuf,sizeof(logbuf));
	_LOG_NOTICE("%s",logbuf);
	/**
	 * if can not push the req, we MUST free the memory
	 * but Do not need to close the socket
	 */
	/*
	if( crawl_queue.push(req) ){
		free(req);
		return -1;
	}
	*/
	free(req);
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
		const char* usage = "./nullcrawler $which_port_to_bind\n";
		puts(usage);
		return -1;
	}

	int port = atoi(argv[1]);

	ul_logstat_t logstat;

	logstat.events = 0x10; //debug
	logstat.to_syslog = 0; //do not write to syslog
	logstat.spec = 0x0;

	ul_openlog(".", "nullcrawler.", &logstat, 2048);

	signal(SIGPIPE, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

	int sock = ul_tcplisten(port, 5);
	if ( sock < 0 ) {
		_LOG_FATAL("ul_tcplisten failed, %m");
		return -1;
	}

	_LOG_NOTICE("CRAWLER listen successfully ...");

	init_conn_mgr(sock);

	do_receive();

	assert(0);

	return 0;
}
