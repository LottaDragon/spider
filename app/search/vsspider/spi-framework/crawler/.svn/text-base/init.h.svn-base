#ifndef __INIT_H__
#define __INIT_H__

#include "ul_log.h"
#include "com_log.h"

// 传输到ab模块的分割字符串
#define _SPLIT "~@@@~"

#define _LOG_FATAL(fmt, arg...) do {ul_writelog(UL_LOG_FATAL, \
		"[%s:%d] [%s] : "fmt, __FILE__,__LINE__, __func__, ##arg);} while(0)

#define _LOG_WARNING(fmt, arg...) do {ul_writelog(UL_LOG_WARNING, \
		"[%s:%d] [%s]: "fmt, __FILE__,__LINE__, __func__, ##arg);} while(0)

#define _LOG_NOTICE(fmt, arg...) do {ul_writelog(UL_LOG_NOTICE, \
		"[%s]: "fmt, __func__, ##arg);} while(0)

#define _LOG_TRACE(fmt, arg...) do {ul_writelog(UL_LOG_TRACE, \
		"[%s] :"fmt, __func__, ##arg);} while(0)

#define _LOG_DEBUG(fmt, arg...) do {ul_writelog(UL_LOG_DEBUG, \
		"[%s] [%s] : "fmt, __FILE__, __func__, ##arg);} while(0)

#define _LOG_NET(fmt, arg...) do { com_writelog("AB", "[%s] :"fmt, \
		__func__, ##arg);} while(0)


// 配置项最大长度
#define MAX_CONF_STR_LEN			256

// 默认crawler ip
#define DEFAULT_CRW_IP				"127.0.0.1"
// 默认crawler port
#define	DEFAULT_CRW_PORT			7778

// 默认最大连接
#define	DEFAULT_MAX_CONN_NUM		10000
// 默认并发连接数
#define DEFAULT_THREAD_NUM			6
// 默认最大htmlpage的size
#define	DEFAULT_MAX_PAGE_LEN		(256*1024) 
// 默认阻塞队列大小
#define DEFAULT_BLOCK_QUEUE_LIMIT	15000

// 都是针对抓取的超时
// 默认连接超时大小
#define DEFAULT_CONN_TIMEOUT		1		//1秒
// 默认读超时大小
#define	DEFAULT_READ_TIMEOUT		10		//10秒
// 默认写超时大小
#define DEFAULT_WRTE_TIMEOUT		1		//1秒

// 默认dns服务器地址
#define DEFAULT_DNS_SERVER			"db-spi-dns4.db01.baidu.com"
// 默认dns服务端口
#define DEFAULT_DNS_UDP_PORT		1280	

// 默认重定向最大次数
#define DEFAULT_REDIR_TIMES			4

// 默认最多重试次数
#define DEFAULT_RETRY_TIMES			2

// 默认不往ab写
#define DEFAULT_LOG_NET				0

// 默认user-agent
#define DEFAULT_USER_AGENT			"Baiduspider+(+http://www.baidu.com/search/spider.htm)"

// 配置项结构体
struct config_t{
	char	listen_ip[MAX_CONF_STR_LEN];
	int		listen_port;

	int		thread_num;
	int		max_conn_num;
	int		max_page_len;
	int 	block_queue_limit;

	char	user_agent[MAX_CONF_STR_LEN];
	int		conn_timeout;
	int		read_timeout;
	int 	write_timeout;

	char	dns_server[MAX_CONF_STR_LEN];
	int		dns_udp_port;

	int		redir_times;
	int 	retry_times;

	int		s_robots_disable;
	int 	s_robots_real;

	int		log2ab;

	char	ab_log_path[MAX_CONF_STR_LEN];
};

// 全局配置项
extern config_t g_config;

// 全局运行flag
extern volatile int g_running;

// extern ul_logstat_t g_logstat;

// 环境初始化
int initial();


#endif
