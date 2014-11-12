#ifndef __RECEIVER_H__
#define __RECEIVER_H__

#include "stdint.h"

#include "block_queue.h"
//#include "../public/vs-spider.h"
#include "vs-spider.h"


#define MAX_CRAWL_PACK_LEN   (16*1024)


//抓取的请求结构体
struct req_crawl_t{
	char				url[MAX_URL_LEN];
	uint32_t 			session_id;
	char				ip[32];
	int					port;
	char				method[64];			//GET|HEAD|POST
	char				protocol[64];		//HTTP1.0|HTTP1.1
	char				cookie[MAX_COOKIE_LEN];
	char				referrer[MAX_REFERRER_LEN];
	char				pp_ip[32];
	int					pp_port;

	// 表明当前链接是否遵守robots协议，非0不遵守
	int					s_robots_disable;
	// 如果s_robots_real非0, 代表被robots.txt屏蔽
	int 				s_robots_real;		
	
	int					buf_len;				//buffer length
	char				req_buff[MAX_CRAWL_PACK_LEN];	//mc_pack的buffer
	
	void show()
	{
		fprintf(stdout,"url:%s\n",url);	
		fprintf(stdout,"ip:%s\n",ip);
		fprintf(stdout,"port:%d\n",port);
		fprintf(stdout,"method:%s\n",method);
		fprintf(stdout,"protocol:%s\n",protocol);
		fprintf(stdout,"cookie(old):%s\n",cookie);
		fprintf(stdout,"referer:%s\n",referrer);
		fprintf(stdout,"pp_ip:%s\n",pp_ip);
		fprintf(stdout,"pp_port:%d\n",pp_port);
		fprintf(stdout,"buf_len:%d\n",buf_len);
	}

	char* to_string(char* buf, int buflen)
	{
		snprintf(buf,buflen,"url:%s,ip:%s,port:%d,method:%s,http:%s,cookie:%s,"
			"referer:%s,pp_ip:%s,pp_port:%d",url,ip,port,method,protocol,cookie,
			referrer,pp_ip,pp_port);	
		buf[buflen-1]=0;
		return buf;
	}
};


extern block_queue_t<req_crawl_t*> crawl_queue;

// receive new connection or data
void do_receive();

// init the connection metainfo manager
void init_conn_mgr(int listen_sock);

#endif
