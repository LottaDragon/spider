/* File: crawler_header.h
 * Author: yue yifan
 * crawler common header
 * Create:   10/05/2008
 * LastMod:  
 **/

#ifndef _CRAWLER_HEADER_H_
#define _CRAWLER_HEADER_H_

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
#include <signal.h>
#include <pthread.h>

#ifdef KERNEL24
#include "epoll.h"
#else
#include <sys/epoll.h>
#endif

#include "ul_log.h"
#include "ul_net.h"
#include "bsl/containers/hash/bsl_hashmap.h"

#include "nshead.h"
#include "mc_pack.h"

//#include "../public/vs-spider.h"
#include "vs-spider.h"

#include "init.h"
#include "receiver.h"

#define MAX_BUFF_LEN       8192
#define MAX_HTML_LEN			(1024*1024)
#define MAX_LINE_LEN       1024

#define	MAX_PACK_LEN			(1200*1024)

#define MAX_PAGE_ITEM      256

#define MAX_HTTP_HEAD_LEN  4096

// 非堵塞读写http response数据包的状态
enum {
	CRAWL_NONE = 0,
	CRAWL_CONNECT,		  //url连接状态
	CRAWL_WRITE_CMD,	  //写http request状态 

	CRAWL_READ_HEAD,      //读html response的head
	CRAWL_READ_BODY,      //读html的页面content
};

// result类型
enum Result_Type{
	ERR_BAD_URL = 0,
	ERR_DNS_RESOLVE,
	ERR_CONN,
	ERR_CONN_TIMEOUT,
	ERR_WRITE_FAIL,
	ERR_WRITE_TIMEOUT,
	ERR_READ_TIMEOUT,
	ERR_READ_FAIL,
	ERR_PEER_CLOSE,
	ERR_5XX,
	ERR_4XX,
	ERR_REDIR,
	ERR_OTHER,	

	RES_3XX,
	RES_2XX,

	ERR_ROBOTS	
};

// result对应的名字
static const char* Result_Name[] = {
	"Bad url",
	"DNS resolve failed",
	"Connection failed",
	"Connection timeout",
	"Write http req failed",
	"Write http request timeout",
	"Read html buffer timeout",
	"Read failed",
	"Peer close",
	"5xx Server side error",
	"4xx Maybe no such file",
	"Redir error",
	"Other error, maybe epoll error, omg!",
	"Succ, 3xx and redirected",
	"Succ, 2xx and get page",
	"Robots forbiden"
};

// http response的metadata
struct http_res_t{
	int		result;						//告诉PP, 对应Result_type
	int 	statu;						//2xx/3xx/4xx/5xx
	char	last_mod[MAX_PAGE_ITEM];	//last modfify time
	int 	cont_len;					//content length
	char	cont_type[MAX_PAGE_ITEM];	//text/* image/*
	char	charset[MAX_PAGE_ITEM];		//gbk gb2312 utf-8
	char	redir[MAX_URL_LEN];			//redir url
	char	etag[MAX_PAGE_ITEM];		//etag
	char	cookie[MAX_COOKIE_LEN];		//cookie
	char	cache_control[MAX_PAGE_ITEM];
	char	expires[MAX_PAGE_ITEM];

	void show()
	{
		fprintf(stdout,"statu:%d\n",statu);	
		fprintf(stdout,"last_mod:%s\n",last_mod);	
		fprintf(stdout,"cont_len:%d\n",cont_len);	
		fprintf(stdout,"cont_type:%s\n",cont_type);	
		fprintf(stdout,"charset:%s\n",charset);	
		fprintf(stdout,"redir:%s\n",redir);	
		fprintf(stdout,"etag:%s\n",etag);	
		fprintf(stdout,"cookie:%s\n",cookie);	
		fprintf(stdout,"cache_control:%s\n",cache_control);	
		fprintf(stdout,"expires:%s\n",expires);	
	}

	void reset()
	{
		result = 0;
		statu = 0;
		last_mod[0] = 0;
		cont_len = 0;
		cont_type[0] = 0;
		charset[0] = 0;
		redir[0] = 0;
		etag[0] = 0;
		cookie[0] = 0;
		cache_control[0] = 0;
		expires[0] = 0;
	}

	// 不复位redir
	void reset_ex()
	{
		result = 0;
		statu = 0;
		last_mod[0] = 0;
		cont_len = 0;
		cont_type[0] = 0;
		charset[0] = 0;
		//redir[0] = 0;
		etag[0] = 0;
		cookie[0] = 0;
		cache_control[0] = 0;
		expires[0] = 0;
	}
};

// 用于读取http response header的buffer结构体，用于网络IO
struct io_buffer_t
{
	int  read_pos;
	int  buff_len;
	char buff[MAX_BUFF_LEN];

	io_buffer_t():read_pos(0),buff_len(0){	buff[0] = 0 ;}
	~io_buffer_t(){}
	void reset()
	{
		read_pos =0;
		buff_len =0;
		buff[0] = 0;
	} 
};

// 用于写入http request的buffer结构体
struct cmd_buffer_t{
	int write_pos;
	int buff_len;
	char buff[MAX_BUFF_LEN];

	cmd_buffer_t():write_pos(0),buff_len(0){ buff[0] = 0 ; }
	~cmd_buffer_t(){}
	void reset()
	{
		write_pos =0;
		buff_len =0;
		buff[0] = 0;
	} 
};

// 每一个连接对应一个page_t，包括http请求信息，http头部的response，当前的抓取状态，抓取buffer
struct page_t{
	req_crawl_t*	req;			//req请求指针，连接关闭以后要free掉

	http_res_t		res;			//http头部的response

	char*			content;		//存page的buffer,连接关闭以后要free掉,lasy malloc

	int				cont_pos;		//buffer位置，正常情况下，读完以后,cont_len==cont_pos

	int64_t			conn_start;		//连接开始时间
	int64_t			write_start;	//写http request开始时间	
	int64_t			read_start;		//读http response开始时间	

	int				crawl_statu; 	//状态

	int				fd;			    //该page所在的fd
	
	int 			redir_num;		//重定向次数

	char			redir_trace[2*MAX_URL_LEN]; 	//重定向trace

	cmd_buffer_t	cmd_buf;		//写command的buffer
	io_buffer_t		read_buf;		//读header的buffer

	void show()
	{
		fprintf(stdout,"++++++++++++++++++++++++++++++\n");
		req->show();
		fprintf(stdout,"\n");
		res.show();
		fprintf(stdout,"\n");
		fprintf(stdout,"cont_pos:%d\n",cont_pos);	
		fprintf(stdout,"start_time:%ld\n",conn_start);
		fprintf(stdout,"crawl_statu:%d\n",crawl_statu);
		fprintf(stdout,"fd:%d\n",fd);
		
		write(1,"++++html++++\n",strlen("++++html++++\n"));
		write(1,content,cont_pos);
		write(1,"\n",1);

		write(1,"++++command++++\n",strlen("++++command++++\n"));
		write(1,cmd_buf.buff,cmd_buf.buff_len);
		write(1,"\n",1);

		write(1,"++++response++++\n",strlen("++++response++++\n"));
		write(1,read_buf.buff,read_buf.read_pos);
		write(1,"\n",1);

		fprintf(stdout,"++++++++++++++++++++++++++++++\n");
	}
};

/**
 * 抓取池，所有的抓取逻辑都在这里
 */
class epool{
	private:

#if 0
		pthread_t fill_thread;

		// 用于finish_read
		char* pack_buff1;
	
		// 用于finish_write
		char* pack_buff2;

		page_t* ps;

		// 限制连接数的lock
		pthread_mutex_t fill_mutex;
		pthread_cond_t fill_cond ;

		// 限制超时管理map操作的lock
		pthread_mutex_t map_mutex;

		// 发送给pp模块 用于finish_read
		int send_2_pp1(page_t* ppage);

		// 发送给pp模块 用于finish_write
		int send_2_pp2(page_t* ppage);

#endif
		// for mcpack2
		//char tmpbuf[8192];

		pthread_t crawl_thread;
		int	epfd;
		struct epoll_event *events; 

		char* pack_buff;

		typedef bsl::hashmap<int, void*> sock_map_t;

		// socket -> user-defined data pointer
		sock_map_t sock_map;

		// 最大并发链接数
		int max_conn;

		// 当前连接数
		volatile int con_num;

		// 用于check_item，判断超时用
		int64_t last_check_time;

		/**    
		 * @brief 抓取结果无论成功失败都发送给PP 
		 *
		 * @param [in] ppage  
		 * @param [in] now : 当前时间 
		 * @return  发送是否成功
		 * @retval  -1 失败；0 成功   
		 **/   
		int send_2_pp(page_t* ppage, int64_t now );

		/**    
		 * @brief  读完抓取了，无论成功还是失败
		 *
		 * @param [in] ppage  
		 * @param [in] now : 当前时间 
		 * @retval 0 
		 **/   
		int finish_read(page_t* ppage, int64_t now);

		/**    
		 * @brief 还没写完就失败了
		 *
		 * @param [in] ppage  
		 * @param [in] now : 当前时间 
		 * @retval 0 
		 **/   
		int	finish_write(page_t* ppage, int64_t now);

		/**    
		 * @brief 重定向抓取 
		 *
		 * @param [in] ppage  
		 * @param [in] now : 当前时刻
		 * @return 重定向抓取是否成功
		 * @retval -1失败，0 成功
		 **/   
		int	handle_redir(page_t* ppage, int64_t now);

		/**    
		 * @brief 从block_queue取若干个请求并把连接加入到epoll中
		 * @param [in] now : 当前时刻
		 * @retval 0 
		 **/   
		int add_n_sock(int64_t now);

		/**    
		 * @brief 添加单个连接到epoll中
		 *
		 * @param [in] ppage  
		 * @param [in] now : 当前时刻
		 * @return 添加是否成功
		 * @retval -1失败，0 成功
		 **/   
		int add_1_sock(page_t* ppage, int64_t now);

		/**    
		 * @brief 主要用来检查超时
		 *        包括连接超时 写命令超时 读超时
		 *
		 * @param [in] now : 当前时刻
		 * @retval 0
		 **/   
		int	check_item(int64_t now);	

		/**    
		 * @brief 生成发送到ab的统计信息
		 *
		 * @param [in] ppage  
		 * @param [in/out] buff: 统计信息存储buffer
		 * @param [in] buflen: buffer长度
		 * @param [in] now: 当前时间
		 * @return 生成是否成功 
		 * @retval -1失败，0 成功
		 **/   
		int build_ab_stats_buff(page_t *ppage, char *buff, size_t buflen, int64_t now);

	public:
		//epool(int _max_conn);
		epool();
		~epool();
			
		int is_running;
		
		/**    
		 * @brief 初始化 
		 *
		 * @param [in] _max_conn : 最大连接数目  
		 * @return 初始化能否成功
		 * @retval -1失败，0 成功
		 **/   
		int init(int _max_conn);

		/**    
		 * @brief 处理所有的连接，读/写/超时管理等
		 *
		 * @return 是否成功
		 * @retval -1失败，0 成功
		 **/   
		int handle_sock();	

		/**    
		 * @brief 开始运行 
		 *
		 **/   
		void do_run();

		/**    
		 * @brief 等待线程结束 
		 *
		 **/   
		void wait_end();

		/**    
		 * @brief 抓取池当前状态 
		 *
		 **/   
		void show();
};
extern int connect_pp(req_crawl_t* req);
#endif
