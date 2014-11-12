/**
 * 读取url的文件列表并进行抓取
 * 1. 验证抓取功能
 * 2. 可用于线下一些调研，但是这里需要控制抓取速度，否则可能会抓死网站的 
 *
 */

#include "init.h"
#include "util.h"
#include "handler.h"
#include "receiver.h"
#include "ipdict.h"

int	fill_config()
{

#if 0
	g_logstat.events = UL_LOG_DEBUG; //debug
	g_logstat.to_syslog = 0; //do not write to syslog
	g_logstat.spec = 0x0;

	ul_openlog(".", "filecrawler.", &g_logstat, 2048);

#endif

	int ret = com_loadlog(".", "comlog.conf");
	if( ret < 0){
		return -1;
	}

	g_config.max_conn_num = DEFAULT_MAX_CONN_NUM;

	g_config.thread_num = 1 /*DEFAULT_THREAD_NUM*/;

	g_config.max_page_len = DEFAULT_MAX_PAGE_LEN;

	strncpy(g_config.user_agent, DEFAULT_USER_AGENT, MAX_CONF_STR_LEN);
	g_config.user_agent[MAX_CONF_STR_LEN-1] = 0;

	g_config.conn_timeout = 10/*DEFAULT_CONN_TIMEOUT*/;

	g_config.read_timeout = DEFAULT_READ_TIMEOUT;

	g_config.write_timeout = DEFAULT_WRTE_TIMEOUT; 

	strncpy(g_config.dns_server, DEFAULT_DNS_SERVER, MAX_CONF_STR_LEN);
	g_config.dns_server[MAX_CONF_STR_LEN-1] = 0;

	g_config.dns_udp_port = DEFAULT_DNS_UDP_PORT; 

	g_config.redir_times = DEFAULT_REDIR_TIMES;

	g_running = 1;

	signal(SIGHUP, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);

	return 0;		
}

int main(int argc, char **argv)
{

	if( argc != 4){
		fprintf(stdout,"./filecrawler $url_file_path $pp_ip $pp_port\n");
		return 1;
	}

	fill_config();
	assert( init_ipdict() == 0 );

	int pp_port = atoi(argv[3]);

	FILE* fp = fopen(argv[1],"r");
	if( !fp){
		_LOG_WARNING("can not open file[%s].%m",argv[1]);	
		return 1;
	}

	epool *pool = new epool[ g_config.thread_num ];
//	epool *pool = (epool*)malloc( g_config.thread_num*sizeof(epool) );
	for(int i =0; i < g_config.thread_num; i++){
		assert( pool[i].init( g_config.max_conn_num) == 0 );
		pool[i].do_run();
	}

	char line[4096];

	char pack_buff[16*1024];
	char tmpbuf[10240];

	int cnt = 0;
	while( get_line(fp,line,sizeof(line)) == 0 ){
		req_crawl_t *req = (req_crawl_t *)calloc(1,sizeof(req_crawl_t));

		mc_pack_t *pp = mc_pack_open_w(2, pack_buff, sizeof(pack_buff), tmpbuf, sizeof(tmpbuf));
		//mc_pack_t *pp = mc_pack_create( pack_buff, sizeof(pack_buff) );                                          
		if( MC_PACK_PTR_ERR(pp) ){         
			continue;
		}
		
		/*
		if( ! mc_pack_valid(pp, sizeof(pack_buff))){
			continue;
		}
		*/
		
		mc_pack_put_uint32(pp,"Pack_type", PACK_CC2CR/*PACK_LINKDB_SEEK*/);
		mc_pack_put_uint32(pp,"session_id",11);
		mc_pack_put_int32(pp,"site_id",11);
		mc_pack_put_str(pp,"url",line);

		if( mc_pack_pack2text(pp,req->req_buff,sizeof(req->req_buff),0) ){
			continue;
		}

		strncpy( req->url, line, MAX_URL_LEN);
		req->url[MAX_URL_LEN-1] = 0;

		strcpy(req->method,"GET");

		strcpy(req->protocol,"HTTP/1.0");

		req->ip[0]=0;

		req->port=0;

		req->cookie[0] =0;

		req->referrer[0] =0;

		strncpy(req->pp_ip,argv[2],32);
		req->pp_ip[31]=0;

		req->pp_port = pp_port;

		if(crawl_queue.push(req)){
			free(req);
			req = NULL;
			_LOG_WARNING("cout not push crawl request url[%s]",line);
		}
		//Nanosleep(200);

		sleep(2);

		cnt++;
		if( cnt % 100 == 0){
			_LOG_NOTICE("push %d url to crawl",cnt);
		}
	}
	_LOG_NOTICE("totally push %d url to crawl",cnt);

	for(int i =0; i < g_config.thread_num; i++){
		pool[i].wait_end();
	}

	return 0;
}
