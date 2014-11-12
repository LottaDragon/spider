#include "ul_net.h"


#include "init.h"
#include "util.h"
#include "ipdict.h"


// 初始化词典大小
const int DICT_SIZE = 90001;

// udp查找时候的timeout,单位 ms
const int DNS_TIMEOUT = 250; 

// ip词典
static ip_dict_t g_ipdict;

// dns服务器地址
static sockaddr_in g_serv_addr;

/**
 * @brief 词典初始化
 * @return int 初始化结果
 * @retval -1失败；0成功
 **/
int init_ipdict(){
	if( g_ipdict.create(90001) != 0){
		_LOG_FATAL("Create IP dict fail.");
		return -1;
	}

	bzero(&g_serv_addr, sizeof(sockaddr_in));
	if (ul_gethostipbyname_r(g_config.dns_server, &g_serv_addr, BD_NET, NULL) != 0){  
		_LOG_FATAL("Resolve DNS server name (%s) fail. %m", g_config.dns_server);
		return -1;
	}  

	g_serv_addr.sin_family = AF_INET;
	g_serv_addr.sin_port = htons( g_config.dns_udp_port);

	return 0;
}

/**
 * @brief url转化给ip
 * 
 * @param [in] url
 * @param [out] ip
 * @param [in] ip_len
 * @return int 是否获取ip信息
 * @retval  -1 失败；0 成功
 **/
int url2ip(const char *url, char *ip, int ip_len)
{
#if 0
	assert(url);
	assert(ip);
#endif

	if( url == NULL || ip == NULL || ip_len <= 0 ){
		_LOG_WARNING("Not valid argument.");
		return -1;
	}

	spi_url_t spi_url;
	if( spi_url_parse( url, &spi_url ) != 0 ){
		_LOG_WARNING( "[URL:%s] is a bad url", url);
		return -1;
	}

	//char site[256];
	//if( ul_get_site_ex(url,site,256) ){

	return site2ip(spi_url.site,ip,ip_len);
}

/**
 * @brief for debug
 */
void info_ipdict()
{
	for( ip_dict_t::iterator itrt = g_ipdict.begin();
			itrt != g_ipdict.end();
			itrt ++ ){
		printf("site:%s,ip:%s,updatetime:%u\n",
				itrt->first.c_str(),
				itrt->second.ip,
				(unsigned)itrt->second.last_update_time);			 
	}	
}

int site2ip(const char *site, char *ip, int ip_len)
{
	if( site == NULL || ip == NULL || ip_len <= 0 ){
		_LOG_WARNING("Not valid argument.");
		return -1;
	}

	bstring str(site);

	time_t now = time(NULL);
	siteinfo_t siteinfo;
	init_siteinfo(siteinfo);

	std::pair<bstring,ip_node> *ppair = NULL;
	ppair = g_ipdict.find(str);
	if( ppair){
		//一天没有更新dns信息了，立即更新
		int diff = now - (*ppair).second.last_update_time;
		if( diff > 24*60*60 ){

			int ret = spidns_seeksite_udp((sockaddr*)(&g_serv_addr),sizeof(g_serv_addr),
					site,siteinfo,DNS_TIMEOUT);
			
			if( ret == 0 ){
				// 1.update and cache siteinfo for robots
				clear_siteinfo((*ppair).second.siteinfo);
				(*ppair).second.siteinfo = siteinfo;

				// 2.if ip info existed, update and cache ip 
				if( siteinfo.ip_num > 0){
					struct in_addr addr;
					addr.s_addr = siteinfo.ip[0];
					char iptmp[64];
					if( inet_ntop(AF_INET,&addr,iptmp, INET_ADDRSTRLEN) ){
						_LOG_NOTICE("site[%s],ip change from[%s] to[%s]",
								site, (*ppair).second.ip, iptmp);
						strncpy( (*ppair).second.ip,iptmp,32);
						(*ppair).second.ip[31]=0;

						(*ppair).second.last_update_time = now;

						strncpy(ip,iptmp,ip_len);	
						ip[ip_len-1]=0;

						return 0;
					}else{
						_LOG_WARNING("inet_ntop failed! ip[%u]. %m",siteinfo.ip[0]);
					}
				}

			}else{
				_LOG_WARNING("can not sync the ip, [site:%s][oldip:%s]",
						site,(*ppair).second.ip);
			}
		}	

		// return the old ip of the url
		strncpy(ip, (*ppair).second.ip, ip_len);	
		ip[ip_len-1]=0;

		return 0;

	}else{
		ip_node node;
		memset(&node,0,sizeof(ip_node));
		node.last_update_time = now;

		int ret = spidns_seeksite_udp((sockaddr*)(&g_serv_addr),sizeof(g_serv_addr),
				site,siteinfo,DNS_TIMEOUT);

		if( ret == 0 ){

			node.siteinfo = siteinfo;

			if( siteinfo.ip_num > 0){
				struct in_addr addr;
				addr.s_addr = siteinfo.ip[0];
				if( inet_ntop(AF_INET, &addr, node.ip, INET_ADDRSTRLEN) ){

					g_ipdict.set(str,node);

					_LOG_NOTICE("site[%s], first add in dict ip[%s]", site, node.ip);

					strncpy(ip,node.ip,ip_len);	
					ip[ip_len-1]=0;

					return 0;
				}else{
					_LOG_WARNING("inet_ntop failed! ip[%u]. %m",siteinfo.ip[0]);
				}
			}

		}else{
			_LOG_WARNING("can not sync the ip, [site:%s]",site);
		}

		return -1;
	}

}

int is_robots_banned(const char *url)
{
	assert(url);
	
	char ip[32];
	
	int r;

	int ret = get_ip_and_robots(url,ip,sizeof(ip),&r);
	if( ret == 0 && r == 1){
		return 1;
	}

	return 0;
}

int get_ip_and_robots(const char *url, char *ip, int ip_len, int *robots)
{
	assert(url);
	assert(ip);
	assert(ip_len >0 );
	assert(robots);

	*robots = 0;
	// trans to the shit format for that spidns not support long string
	char s_robots[256];
	strncpy (s_robots, url, sizeof(s_robots)); 
	s_robots[255] = 0;

	spi_url_t spi_url;
	if( spi_url_parse( url, &spi_url ) != 0 ){
		_LOG_WARNING( "[URL:%s] is a bad url", url);
		return -1;
	}
	
	char *site = spi_url.site;
	bstring str(site);

	time_t now = time(NULL);
	siteinfo_t siteinfo;
	init_siteinfo(siteinfo);

	std::pair<bstring,ip_node> *ppair = NULL;
	ppair = g_ipdict.find(str);
	if( ppair){
		//一天没有更新dns信息了，立即更新
		int diff = now - (*ppair).second.last_update_time;
		if( diff > 24*60*60 ){

			int ret = spidns_seeksite_udp((sockaddr*)(&g_serv_addr),sizeof(g_serv_addr),
					site,siteinfo,DNS_TIMEOUT);
			
			if( ret == 0 ){
				// 1.update and cache siteinfo for robots
				clear_siteinfo((*ppair).second.siteinfo);
				(*ppair).second.siteinfo = siteinfo;

				// 2.if ip info existed, update and cache ip 
				if( siteinfo.ip_num > 0){
					struct in_addr addr;
					addr.s_addr = siteinfo.ip[0];
					char iptmp[64];
					if( inet_ntop(AF_INET,&addr,iptmp, INET_ADDRSTRLEN) ){
						_LOG_NOTICE("site[%s],ip change from[%s] to[%s]",
								site, (*ppair).second.ip, iptmp);
						strncpy( (*ppair).second.ip,iptmp,32);
						(*ppair).second.ip[31]=0;

						(*ppair).second.last_update_time = now;
						
						// set ip
						strncpy(ip,iptmp,ip_len);	
						ip[ip_len-1]=0;

						// set robots
						int robots_rule = spidns_checkrobots(s_robots, strlen(s_robots), siteinfo);
						switch (robots_rule) {
							case ROBOTS_RULE_FORBIDEN:
								*robots = 1;
								break;
							case ROBOTS_RULE_GRANTED:
							case ROBOTS_RULE_ERROR:
								break;
							default:
								assert(0);
						}

						return 0;

					}else{
						_LOG_WARNING("inet_ntop failed! ip[%u]. %m",siteinfo.ip[0]);
					}
				}

			}else{
				_LOG_WARNING("can not sync the ip, [url:%s][site:%s][oldip:%s]",
						url,site,(*ppair).second.ip);
			}
		}	

		// return the old ip of the url
		strncpy(ip, (*ppair).second.ip, ip_len);	
		ip[ip_len-1]=0;

		// set the old robots
		int robots_rule = spidns_checkrobots(s_robots, strlen(s_robots), (*ppair).second.siteinfo);
		switch (robots_rule) {
			case ROBOTS_RULE_FORBIDEN:
				*robots = 1;
				break;
			case ROBOTS_RULE_GRANTED:
			case ROBOTS_RULE_ERROR:
				break;
			default:
				assert(0);
		}

		return 0;

	}else{
		ip_node node;
		memset(&node,0,sizeof(ip_node));
		node.last_update_time = now;

		int ret = spidns_seeksite_udp((sockaddr*)(&g_serv_addr),sizeof(g_serv_addr),
				site,siteinfo,DNS_TIMEOUT);

		if( ret == 0 ){

			node.siteinfo = siteinfo;

			if( siteinfo.ip_num > 0){
				struct in_addr addr;
				addr.s_addr = siteinfo.ip[0];
				if( inet_ntop(AF_INET, &addr, node.ip, INET_ADDRSTRLEN) ){

					// insert table
					g_ipdict.set(str,node);

					_LOG_NOTICE("site[%s], first add in dict ip[%s]", site, node.ip);

					// set ip
					strncpy(ip,node.ip,ip_len);	
					ip[ip_len-1]=0;
					
					// set robots
					int robots_rule = spidns_checkrobots(s_robots, strlen(s_robots), siteinfo);
					switch (robots_rule) {
						case ROBOTS_RULE_FORBIDEN:
							*robots = 1;
							break;
						case ROBOTS_RULE_GRANTED:
						case ROBOTS_RULE_ERROR:
							break;
						default:
							assert(0);
					}

					return 0;
				}else{
					_LOG_WARNING("inet_ntop failed! ip[%u]. %m",siteinfo.ip[0]);
				}
			}

		}else{
			_LOG_WARNING("can not sync the ip, [url:%s][site:%s]",
					url,site);
		}

		return -1;
	}
}
