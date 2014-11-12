#include <signal.h>

#include "ul_conf.h"
#include "init.h"
#include "ipdict.h"

volatile int g_running=1;
config_t g_config;

// 信号处理函数
static void handleSIGTERM(int sign_no)
{
	g_running = 0;
}

// 信号初始化
static int init_signal()
{
	signal(SIGHUP, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTERM, handleSIGTERM);
	return 0;
}

// 加载配置文件
static int	load_config()
{
	int ret;
	Ul_confdata *ptr_conf = NULL;
	ptr_conf = ul_initconf(50);

	if( ptr_conf == NULL){
		return -1;
	}

	ret = ul_readconf("./conf/","crawler.conf",ptr_conf);
	if( ret < 0){
		return -1;
	}

	ret = com_loadlog("./conf", "crwlog.conf");
	if( ret < 0){
		return -1;
	}

#if 0
	ret = ul_getconfint(ptr_conf, "LOG_EVENTS", &(g_logstat.events));
	ret = ul_getconfint(ptr_conf, "LOG_TO_SYS", &(g_logstat.to_syslog));
	ret = ul_getconfint(ptr_conf, "LOG_SPEC", &(g_logstat.spec));	
	ul_openlog("./log/", "vsspider.crw.", &g_logstat, 2048);
#endif

	if( ! ul_getconfstr(ptr_conf,"LISTEN_IP",g_config.listen_ip) ){
		strncpy(g_config.listen_ip, DEFAULT_CRW_IP, MAX_CONF_STR_LEN);
		g_config.listen_ip[MAX_CONF_STR_LEN-1] = 0;
	}

	if( ! ul_getconfint(ptr_conf,"LISTEN_PORT",&(g_config.listen_port) ) ){
		g_config.listen_port = DEFAULT_CRW_PORT;
	}	

	if( ! ul_getconfint(ptr_conf,"MAX_CONN_NUM", &(g_config.max_conn_num) ) ){
		g_config.max_conn_num = DEFAULT_MAX_CONN_NUM;
	}	

	if( ! ul_getconfint(ptr_conf,"THREAD_NUM", &(g_config.thread_num) ) ){
		g_config.thread_num = DEFAULT_THREAD_NUM;
	}	

	if( ! ul_getconfint(ptr_conf,"BLOCK_QUEUE_LIMIT", &(g_config.block_queue_limit) ) ){
		g_config.block_queue_limit = DEFAULT_BLOCK_QUEUE_LIMIT;
	}	

	if( ! ul_getconfint(ptr_conf,"MAX_PAGE_LEN", &(g_config.max_page_len) ) ){
		g_config.max_page_len = DEFAULT_MAX_PAGE_LEN;
	}	

	if( ! ul_getconfstr(ptr_conf,"USER_AGENT",g_config.user_agent) ){
		strncpy(g_config.user_agent, DEFAULT_USER_AGENT, MAX_CONF_STR_LEN);
		g_config.user_agent[MAX_CONF_STR_LEN-1] = 0;
	}

	if( ! ul_getconfint(ptr_conf,"CONN_TIMEOUT", &(g_config.conn_timeout) ) ){
		g_config.conn_timeout = DEFAULT_CONN_TIMEOUT;
	}

	if( !ul_getconfint(ptr_conf,"READ_TIMEOUT", &(g_config.read_timeout)) ){
		g_config.read_timeout = DEFAULT_READ_TIMEOUT;
	}

	if( !ul_getconfint(ptr_conf,"WRITE_TIMEOUT", &(g_config.write_timeout)) ){
		g_config.write_timeout = DEFAULT_WRTE_TIMEOUT; 
	}

	if( !ul_getconfstr(ptr_conf,"DNS_SERVER", g_config.dns_server) ){
		strncpy(g_config.dns_server, DEFAULT_DNS_SERVER, MAX_CONF_STR_LEN);
		g_config.dns_server[MAX_CONF_STR_LEN-1] = 0;
	}

	if( !ul_getconfint(ptr_conf,"DNS_UDP_PORT", &(g_config.dns_udp_port)) ){
		g_config.dns_udp_port = DEFAULT_DNS_UDP_PORT; 
	}

	if( !ul_getconfint(ptr_conf,"REDIR_TIMES", &(g_config.redir_times) ) ){
		g_config.redir_times = DEFAULT_REDIR_TIMES;
	}

	if( !ul_getconfint(ptr_conf,"RETRY_TIMES", &(g_config.retry_times) ) ){
		g_config.retry_times = DEFAULT_RETRY_TIMES;
	}
	
	if( !ul_getconfint(ptr_conf,"s_robots_disable", &(g_config.s_robots_disable) ) ){
		g_config.s_robots_disable = 0;
	}
	
	if( !ul_getconfint(ptr_conf,"s_robots_real", &(g_config.s_robots_real) ) ){
		g_config.s_robots_real = 0;
	}

	if( !ul_getconfint(ptr_conf,"LOGNET", &(g_config.log2ab) ) ){
		g_config.log2ab = DEFAULT_LOG_NET;
	}

	ul_freeconf( ptr_conf );
	return 0;		
}

static void info_config()
{
	_LOG_NOTICE("+++++++++++++++config info++++++++++++++++");	
	_LOG_NOTICE("listen_ip:%s",g_config.listen_ip);
	_LOG_NOTICE("listen_port:%d",g_config.listen_port);
	_LOG_NOTICE("thread_num:%d",g_config.thread_num);
	_LOG_NOTICE("max_conn_num:%d",g_config.max_conn_num);
	_LOG_NOTICE("max_page_len:%d",g_config.max_page_len);
	_LOG_NOTICE("block_queue_limit:%d",g_config.block_queue_limit);
	_LOG_NOTICE("user-agent:%s",g_config.user_agent);
	_LOG_NOTICE("conn timeout:%d",g_config.conn_timeout);
	_LOG_NOTICE("read timeout:%d",g_config.read_timeout);
	_LOG_NOTICE("write timeout:%d",g_config.write_timeout);
	_LOG_NOTICE("dns server:%s",g_config.dns_server);
	_LOG_NOTICE("dns udp port:%d",g_config.dns_udp_port);
	_LOG_NOTICE("redir times:%d",g_config.redir_times);
	_LOG_NOTICE("retry times:%d",g_config.retry_times);
	_LOG_NOTICE("s_robots_disable:%d",g_config.s_robots_disable);
	_LOG_NOTICE("s_robots_real:%d",g_config.s_robots_real);
	_LOG_NOTICE("log2ab:%d",g_config.log2ab);

}

// 初始化
int initial()
{
	if( load_config() != 0 ){
		_LOG_FATAL("load_config failed!");
		return -1;
	}
	
	info_config();

	if( init_signal() != 0 ){
		_LOG_FATAL("init signal failed!");
		return -1;
	}

	if( init_ipdict() != 0 ){
		_LOG_FATAL("init ipdict failed!");	
		return -1;
	}

	return 0;	
}
