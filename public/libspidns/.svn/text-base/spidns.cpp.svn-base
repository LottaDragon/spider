/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: spidns.cpp,v 1.7 2008/10/08 05:31:55 jiangjinpeng Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file spidns.cpp
 * @author chentj(spider@baidu.com)
 * @date 2008/03/05 11:40:56
 * @version $Revision: 1.7 $ 
 * @brief 
 *  
 **/

#include "ul_log.h"
#include "ul_url.h"
#include "spidns_data.h"
#include "spidns_connection.h"
#include "spidns.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

spidns_t*
spidns_init( int cache_size , int mode , const char *dns_host , int dns_port , const char *local_name, 
			 int retry_count )
{
	if ( dns_host == NULL || local_name == NULL )
	{
		return NULL;
	}


	spidns_t* pret=NULL;



	pret = new spidns_t;		  /**< alloc base structure  */
	
	if ( NULL == pret )
	{
		ul_writelog(UL_LOG_FATAL,"[%s]Cannot allocate spidns_t",__FUNCTION__);
		return NULL;
	}	
	

			  /**< init parameters       */
	if ( init_dnsmode( mode, &(pret->mdns_mode) ) < 0 )
	{
		ul_writelog(UL_LOG_FATAL,"[%s]init dns mode failed",__FUNCTION__);
		delete pret;
		return NULL;
	}

	pret->cache_size = cache_size;

	pret->pdns_data = init_dnsdata( cache_size );

	if ( NULL == pret->pdns_data )
	{
		delete pret;
		return NULL;
	}
		
	init_connection( &(pret->mdns_conn) , dns_host ,dns_port , local_name, retry_count);	

	return pret;
}

void 
spidns_destroy( spidns_t* pdns )
{
	if( pdns == NULL )
		return;
	
	//clear the dns_Data

	release_dnsdata( pdns->pdns_data );	
	
	//clear the main structure
	delete pdns;

	return;
}

int 
spidns_pushsite(spidns_t* pdns , const char *sitename , int site_len )
{

	if( pdns == NULL || pdns->pdns_data == NULL || sitename == NULL )
	{
		return -1;
	}

	siteinfo_t tempinfo;
	unsigned int key = get_sitesign( sitename , site_len ); 
	int ret = 0;

	init_siteinfo(tempinfo);

	//add if not mod

	if ( 0 == add_siteinfo_cache( pdns->pdns_data , key , tempinfo ))
	{
		ret = add_buffer( pdns->pdns_data , sitename , site_len );
	}	 


	return ret;

}

int 
spidns_seeksite( spidns_t* pdns , const char *sitename , int site_len , siteinfo_t &pout )
{
	unsigned int key = get_sitesign( sitename , site_len );

	if ( pdns== NULL || pdns->pdns_data == NULL || sitename == NULL)
	{
		return -1;
	}
	
	return seek_siteinfo( pdns->pdns_data , key , pout );

}

//return value: 0    success
//              -1   分配内存失败   
//              -2   连接dns服务器失败
inline int reconnect_dns( dns_conn_t *pdns_conn )
{
	int ret = 0;	
    close_connection( pdns_conn );
    ret = connect_dns( pdns_conn );
	return ret;
}


inline int transformline(char * dnsinfo_line , char *bufline, int buflen)
{
	char sitename[128];   
    char siteip[32];
	unsigned int state;
	char ipregion;
	unsigned int http_version;
	unsigned int site_level;
	unsigned int case_sensitive;
	unsigned int robots_count;
	unsigned int robots_length;
    char robots_data[2048];


	if( 10 != sscanf( dnsinfo_line , "%s %s %u %c %u %u %u %u %u %s\n" ,
							sitename,                                      
							siteip,
							&state,                       
							&ipregion,
							&http_version,      
							&site_level,
							&case_sensitive,
							&robots_count,
							&robots_length,
							robots_data
					))
	{
		fprintf(stderr,"Cannot parse line : %s\n",dnsinfo_line);
		return -1;
	}

	snprintf( bufline, (size_t)buflen , "%s %s %c %u %u %s\r\n" , sitename , siteip , ipregion, case_sensitive,
				robots_count , robots_data);  	


	return 0;
}


int 
spidns_updateline( spidns_t* pdns , char *dnsinfo_line , int len )
{

/*
    Format Of dns Readable Dump file
	
	"%s %s %u %c %u %u %u %u %u %s\n"
	Colum:
    1.sitename                                      
	2.siteip
    3.state                       
	4.ipregion
    5.http_version      
	6.site_level
	7.case_sensitive
	8.robots_count
    9.robots_length       
   10.robotsdata

*/	
	//char line[4096];
	char *start = dnsinfo_line;
	char *end = dnsinfo_line + len ;
	char sitename[128];

	siteinfo_t tempinfo;
	unsigned int sitekey;

	if ( pdns == NULL || dnsinfo_line == NULL )
	{
		return -1;
	}

/*
	if (0 != transformline(dnsinfo_line, line , 4096 ))
	{
		return -1;
	}

	printf("After transform: %s\n",line);

	start = line;
	end = line + strlen( line );
*/

	init_siteinfo(tempinfo);

	if( 0 != parse_dnsdumpline( start , end , sitename , 64 , tempinfo ))
	{
		return -1;
	}

	sitekey = get_sitesign( sitename , (int)strlen( sitename ));

	if( 0 != update_siteinfo_cache( pdns->pdns_data , sitekey , tempinfo ))
	{
		return -1;
	}

	return 0;

}


#define RVBUF_SIZE (1024*1024)

int 
spidns_syncdata( spidns_t* pdns )
{
	int ret = 0;
	char temp[ DNS_MAX_PACK_NUM * DNS_MAX_SITE_LEN + 1  ];
    int templen = DNS_MAX_PACK_NUM * DNS_MAX_SITE_LEN  ;	
	char *rbuf = NULL;
	int  rbuflen = 0;
	int  packnum = 0;
	char *start,*end;
	char sitename[ 64 ];
	unsigned int sitekey = 0;
	siteinfo_t tempinfo;

	dns_pack_t *rpack = NULL;
	dns_pack_t *wpack = NULL;
	
	if ( pdns == NULL || pdns->pdns_data == NULL )
	{
		return -1;
	}
// init buffer 

	init_siteinfo(tempinfo);

	rpack = init_dnspack();
	if ( rpack == NULL )
	{
		return -1;
	}

	wpack = init_dnspack();
	if ( wpack == NULL )
	{
		destroy_dnspack( rpack );
		return -1;
	}
	
	rbuf = new char[RVBUF_SIZE];
	if ( rbuf == NULL )
	{
		destroy_dnspack( rpack );
		destroy_dnspack( wpack );
		return -1;
	}

	if( reconnect_dns( &(pdns->mdns_conn)) < 0 )
	{
		ret = -1;
		goto RETURN;
	}

	if( build_dnspack_head( wpack ,pdns->mdns_mode.ip) <0 )
	{
		ret = -1;
		goto RETURN;
	}

//获得待解析的buf
	while ( ( ret = pop_buffer( pdns->pdns_data , temp , &templen ) )>0 )
	{
		//printf("Sync Buffer : %d : %s\n" , templen, temp );
		
//组包	
		push_dnspack_body( wpack , temp , templen , ret );
		templen = DNS_MAX_PACK_NUM * DNS_MAX_SITE_LEN;
		push_dnspack_body( rpack , rbuf , RVBUF_SIZE , 0 );
//写入DNS
		while( write_dns( &(pdns->mdns_conn) , wpack ) < 0 )
		{
			if( reconnect_dns( &(pdns->mdns_conn)) < 0 )
			{
				ret = -1;
				goto RETURN;
			}
		}
		 
//读取DNS返回值
		if( (rbuflen = read_dns( &(pdns->mdns_conn) , rpack )) < 0 )	
		{
			continue;
		}
		
		rbuf[ rbuflen ] = 0;

//插入结果池

		packnum = get_dnspack_number( rpack );
		if ( packnum < 0 )
		{
			continue;
		}
		
		start = rbuf;
		for( int i = 0 ; i < packnum ; i++)
		{
			end = strstr( start , "\r\n");

			if( end == NULL )
			{
				break;
			}
			end = end + 1;

			switch(pdns->mdns_mode.ip )
			{
				case IP_SINGLE:
					if( 0 != parse_resultline_singleip( start , end , sitename , 64 , tempinfo ))
					{
						start = end + 1;
						continue;
					}
					break;
				case IP_MULTI:
					if( 0 != parse_resultline_multiip( start , end , sitename , 64 ,tempinfo ))
                    {
						start = end + 1;
                        continue;
                    }
                    break;
				default:
					ret = -1;
					ul_writelog(UL_LOG_WARNING,
								"[%s] error ip mode[%d]",__FUNCTION__,pdns->mdns_mode.ip);
					goto RETURN;
			}
			sitekey = get_sitesign( sitename , (int)strlen( sitename ));

			//printf("site: %s key: %u\n",sitename , sitekey);

			if( 0 != update_siteinfo_cache( pdns->pdns_data , sitekey , tempinfo ))
			{
				start = end + 1;
				continue;
			}
			
			start = end + 1;
		}


	}

RETURN:
	if( rpack != NULL )
	{
		destroy_dnspack( rpack );
		rpack = NULL;
	}
	if(wpack != NULL )
	{
		destroy_dnspack( wpack );
		wpack = NULL;
	}
	if( rbuf != NULL)
	{
		delete []rbuf;
		rbuf = NULL;
	}
	close_connection( &(pdns->mdns_conn));

	if( ret >= 0 )
	{
		return 0;
	}
	else
	{
		return -1;
	}
}


int spidns_clear( spidns_t* pdns )
{
	if ( pdns == NULL )
	{
		return -1;
	}
//删除待解析数据
	clear_buffer( pdns->pdns_data );
//删除结果数据
	clear_cache( pdns->pdns_data );

//断开DNS连接
	close_connection( &( pdns->mdns_conn));
	
	return 0;
}


int 
spidns_checkrobots( const char *purl , int ulen , const siteinfo_t& siteinfo )
{
	char site[256];
	char port[256];
	char path[256];
	int allow_count = 0;
	int disallow_count = 0; 
	int ret = 0;
	//vector<robot_t>::iterator iter;

	if( purl == NULL )
	{
		return -1;
	}

	if( 0 == ul_parse_url( purl , site ,  port , path ))
	{
		return -1;
	}

	for ( int i = 0 ; i < siteinfo.robots_data_num ; i++ )
	{
		ret = check_match( path , (int)strlen(path) , siteinfo.robots_data[i] );
		if( ret == ROBOTS_MATCH )
		{
			switch( siteinfo.robots_data[i].type )
			{
				case  ROBOTS_ALLOW_COMMON :
				case  ROBOTS_ALLOW_TEXT :
				case  ROBOTS_ALLOW_REGEX :
					allow_count++;
					break;
				case  ROBOTS_DISALLOW_COMMON :
                case  ROBOTS_DISALLOW_TEXT :
                case  ROBOTS_DISALLOW_REGEX :
                    disallow_count++;
					break;
				default:
					break;
				
			}
		}
	}
	
	if ( disallow_count > 0 )
	{
		if(allow_count > 0)
		{
			return ROBOTS_RULE_GRANTED;
		}
		else
		{
			return ROBOTS_RULE_FORBIDEN;
		}
	
	}
		
	
	return ROBOTS_RULE_GRANTED;
}

/**
 * @brief	获取当前时间
 * @retval	当前时间，以微秒为单位
 * @note	内部通过gettimeofday实现
 * @author	jiangjinpeng
 * @date	2008/09/24
 */
static inline int64_t get_cur_time()
{
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000LL + tv.tv_usec;
}

/**
 * @brief 解析DNS服务器通过UDP接口返回的查询包
 * 
 * @param [in] response_buf	: 存储服务器返回消息的缓冲区
 * @param [in] sitename		: 要解析的站点名
 * @param [in] sitename_len : 站点名的长度
 * @param [out] siteinfo	: 站点信息，包含站点IP，站点robots等信息
 * @retval					: 0=>解析成功，小于0=>解析失败
 * @see
 * @note					: 用于spidns_seeksite_udp函数解析服务器响应 
 * @author jiangjinpeng
 * @date 2008/09/24
 */
static int spidns_parse_udp_response(const char* response_buf, const char* sitename, size_t sitename_len, siteinfo_t& siteinfo) 
{
	istringstream istr(response_buf);

	char rcv_sitename[DNS_MAX_SITE_LEN];
	char rcv_ip[40];
	int robots_count = 0;
	
	istr >> rcv_sitename >> rcv_ip >> robots_count;
	if (!istr || strncmp(sitename, rcv_sitename, sitename_len) != 0 || robots_count < 0)
	{
		ul_writelog(UL_LOG_WARNING, "[spidns_parse_udp_response] illegal response: %s", response_buf);
		return -1;
	}

	clear_siteinfo(siteinfo);

	unsigned int ip = inet_addr(rcv_ip);
	if (ip == 0)
	{
		siteinfo.ip_num = 0;
		siteinfo.ip = NULL;
	}
	else
	{
		siteinfo.ip_num = 1;
		siteinfo.ip = new unsigned int[1];
		siteinfo.ip[0] = ip;
	}


	siteinfo.robots_data_num = 0;
	siteinfo.robots_data = new robot_t[robots_count];
	bzero(siteinfo.robots_data, sizeof(robot_t) * robots_count);

	char robots_buf[1024];
	istr >> robots_buf;
	while (siteinfo.robots_data_num < robots_count && istr)
	{
		int parse_ret = parse_robot(robots_buf, (int)strlen(robots_buf), siteinfo.robots_data[siteinfo.robots_data_num]);
		if (parse_ret < 0)
		{
			ul_writelog(UL_LOG_FATAL, "[spidns_parse_udp_response] parse robot fail: %s", robots_buf);
		}
		else
		{
			++siteinfo.robots_data_num;
		}
		istr >> robots_buf;
	}
	if (siteinfo.robots_data_num < robots_count)
	{
		ul_writelog(UL_LOG_WARNING, "[spidns_parse_udp_response] actual robots count(%d) less than provided(%d): %s"
				, siteinfo.robots_data_num, robots_count, response_buf);
	}
	return 0;
}

/**
 * @brief 通过UDP接口检索DNS信息(包含robots信息)
 * 
 * @param [in] servaddr		: DNS服务器的地址
 * @param [in] addrlen		: DNS服务器地址的长度
 * @param [in] sitename		: 站点名，以\0结束，本函数不负责检查sitename合法性
 * @param [out] siteinfo	: 站点信息，包含站点IP，站点robots等信息
 * @param [in] time			: 毫秒级的超时，默认为0。0表示无限超时
 * @retval					: 0=>查询成功，-1=>查询失败
 * @see
 * @note					: spidns_seeksite_udp仅使用UDP包与DNS服务器通信，不使用libspidns内部的cahce，不使用spidns_t结构
 * @author jiangjinpeng
 * @date 2008/09/24
 */
int spidns_seeksite_udp(const sockaddr* servaddr, socklen_t addrlen, const char* sitename, siteinfo_t& siteinfo, int timeout)
{
	// 检查输入参数
	if (servaddr == NULL || sitename == NULL || addrlen <= 0 || timeout < 0)
	{
		ul_writelog(UL_LOG_FATAL, "[spidns_seeksite_udp] invalid arguments.");
		return -1;
	}

	while (*sitename != '\0' && isspace(*sitename))
	{
		++sitename;
	}
	if (*sitename == '\0')
	{
		ul_writelog(UL_LOG_WARNING, "[spidns_seeksite_udp] empty sitename.");
		return -1;
	}
	size_t sitename_len = 1;
	while (sitename[sitename_len] != '\0' && !isspace(sitename[sitename_len]))
	{
		++sitename_len;
	}
	if (sitename_len >= DNS_MAX_SITE_LEN)
	{
		ul_writelog(UL_LOG_WARNING, "[spidns_seeksite_udp] sitename too long: %s", sitename);
		return -1;
	}
	
	// recvfrom函数使用的地址
	sockaddr* recv_addr = (sockaddr*)alloca(addrlen);
	socklen_t recv_addr_len = addrlen;

	// 创建UDP socket
	int sock = socket(servaddr->sa_family, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == -1)
	{
		ul_writelog(UL_LOG_FATAL, "[spidns_seeksite_udp] create udp socket fail.");
		return -1;
	}

	// 准备查询和响应数据包
	char request_buf[1500] = "2 ";					// 2是版本号
	memcpy(request_buf + 2, sitename, sitename_len);
	request_buf[2 + sitename_len] = 0;
	size_t request_len = 2 + sitename_len + 1;		// 加上最后的\0
	const size_t resp_buf_len = 1500;
	char response_buf[resp_buf_len];

	// 建立用于select的fdset
	fd_set readfds;
	FD_ZERO(&readfds);
	
	// 记录当前时间
	int64_t start_time = get_cur_time();
	int64_t cur_time = start_time;
	int64_t last_send_time = start_time;
	timeout *= 1000;						// 以微秒计算

	while (true)
	{
		// 检测超时
		cur_time = get_cur_time();
		if (timeout != 0 && cur_time - start_time >= timeout)
		{
			break;
		}
		// 向DNS服务器发送UDP查询包
		last_send_time = cur_time;
		while (true)
		{
			ssize_t bytes_sent = sendto(sock, request_buf, request_len, 0, servaddr, addrlen);
			if (bytes_sent != (ssize_t)request_len)
			{
				if (errno == EINTR)		// interrupted by a signal
				{
					continue;
				}
				else
				{
					char error_buffer[1024];
					strerror_r(errno, error_buffer, 1024);
					ul_writelog(UL_LOG_FATAL, "[spidns_seeksite_udp] sendto fail: %s", error_buffer);
					close(sock);
					return -1;
				}
			}
			break;
		}
		
		while (true)
		{
			// 检测socket的可读性
			cur_time = get_cur_time();
			int64_t wait_time = MAX_DNS_RESPONSE_TIME - (cur_time - last_send_time);
			if (timeout > 0)
			{
				int64_t time_left = timeout - (cur_time - start_time);
				wait_time = min(wait_time, time_left);				
			}
			if (wait_time <= 0)
			{
				break;
			}
			timeval tv_wait;
			tv_wait.tv_sec = (time_t)(wait_time / 1000000LL);
			tv_wait.tv_usec = (suseconds_t)(wait_time % 1000000LL);
			FD_SET((unsigned int)sock, &readfds);
			if (select(sock + 1, &readfds, NULL, NULL, &tv_wait) != 1)
			{
				// DNS服务器超过MAX_DNS_RESPONSE_TIME没有正确响应，重新发送请求
				break;
			}
			// 读取解析响应数据包
			int bytes_recved = 0;
			while (true)
			{
				recv_addr_len = addrlen;
				bytes_recved = recvfrom(sock, response_buf, resp_buf_len, 0, recv_addr, &recv_addr_len);
				if (bytes_recved <= 0)
				{
					if (errno == EINTR)
					{
						continue;
					}
					else
					{
						char error_buffer[1024];
						strerror_r(errno, error_buffer, 1024);
						ul_writelog(UL_LOG_FATAL, "[spidns_seeksite_udp] recvfrom fail: %s", error_buffer);
						close(sock);
						return -1;
					}
				}
				break;
			}
			// 源地址检查
			if (addrlen != recv_addr_len || memcmp(servaddr, recv_addr, addrlen) != 0)
			{
				ul_writelog(UL_LOG_WARNING, "[spidns_seeksite_udp] receive response from other host.");
				continue;
			}
			// 解析数据包
			response_buf[bytes_recved] = 0;
			if (spidns_parse_udp_response(response_buf, sitename, sitename_len, siteinfo) != 0)
			{
				ul_writelog(UL_LOG_WARNING, "[spidns_seeksite_udp] parse response for site (%s) fail.", sitename);
				continue;
			}
			// 解析成功
			close(sock);
			return 0;
		}
	}
	ul_writelog(UL_LOG_WARNING, "[spidns_seeksite_udp] DNS lookup for site %s timeout.", sitename);
	// 超过timeout时限
	close(sock);
	return -1;
}

#ifdef TEST_SPIDNS

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <unistd.h>


void 
print_usage()
{
	printf("Usage:\n\ttest_spidns -h dns_host_addr -p dns_host_port -f urlfile [-d debugdnsfile]" 
			"[-m ip_mode] [-n retry_count]\n");
}


int
main( int argc , char *argv[] )
{
	char optc = '0';
	char dns_host[256]="127.0.0.1";
	char pathname[1024];
	char dnsfile[1024];
	int  dns_port= 9100;
	spidns_t *pdns = NULL;
	FILE *fp = NULL;
	FILE *dfp = NULL;
	char site[1024];
	char path[1024];
	char port[1024];
	char url[1024];
	int ret = 0;
	struct in_addr in;

	int use_dns = 1;
	int ip_mode = 1;
	int retry_count = 0;
	siteinfo_t resinfo;
	int i = 0;
	char ip_str[1024];

	if(argc < 3) 
	{
		print_usage();
		return -1;
	}

	while ((optc = getopt(argc, argv, "h:p:f:d:m:n:v")) != -1)
	{
		switch (optc)
		{
			case 'h':
				strcpy(dns_host,optarg);
				break;
			case 'p':
				dns_port = atoi(optarg);
				break;	
			case 'f':
				strcpy(pathname,optarg);
				break;
			case 'd':
				strcpy(dnsfile,optarg);
				use_dns = 0;
				break;
			case 'm':
				ip_mode = atoi(optarg);
				break;
			case 'n':
				retry_count = atoi(optarg);
				break;
			case 'v':
			default:
				print_usage();
				return -1;
		}
	}

//	printf("DNS HOST:%s\n",dns_host);
//	printf("DNS PORT:%d\n",dns_port);
//	printf("URL FILE:%s\n",pathname);

	fp = fopen ( pathname , "r" );
	if ( fp == NULL )
	{
		fprintf( stderr , "Open Url File:%s Failed. EXIT\n" , pathname );	
		return 1;
	}

	pdns = spidns_init( 100000 , ip_mode , dns_host , dns_port, "Test_Client", retry_count );

	if( pdns == NULL )
	{
		fprintf( stderr , "Init DNS Failed\n" );
		return 1;	
	}


	if ( use_dns == 1 )
	{
		while( NULL != fgets( url , 1024 , fp ))
		{
			if( 0 == ul_parse_url( url , site ,  port , path ))
			{   
				continue;
			}
			if(strlen(port)!=0)
			{
				strcat( site , ":");
				strcat( site , port );
			}
			if( spidns_pushsite( pdns , site , strlen(site) ) < 0)
			{
				continue;
			}
		}
		
		
		if( spidns_syncdata(pdns) < 0 )
		{
			fprintf( stderr , "Sync with DNS Failed\n" );
			return 1;
		}


	}
	else
	{
	    dfp = fopen ( dnsfile , "r" );
		if ( dfp == NULL )
		{
			fprintf( stderr , "Open Debug DNS File:%s Failed. EXIT\n" , dnsfile );
			return 1;
		}
		
		while( NULL != fgets( url , 1024 , dfp ))
		{
			spidns_updateline( pdns , url ,strlen( url ));				
		}
		
		
	}

	rewind( fp );

    while( NULL != fgets( url , 1024 , fp ))
    {
		url[ strlen(url) -1 ] = 0;
        if( 0 == ul_parse_url( url , site ,  port , path ))
        {
            continue;
        }
        if(strlen(port)!=0)
        {
             strcat( site , ":");
             strcat( site , port );
        }
        if( spidns_seeksite( pdns , site , strlen(site) , resinfo) < 0)
        {
            continue;
        }
	
		if( resinfo.ip_num <= 0 )
		{
			continue;
		}	

		ip_str[0]=0;
		for( i = 0; i < resinfo.ip_num; i++)
		{
			in.s_addr = resinfo.ip[i];
			strcat(ip_str, inet_ntoa(in));
			strcat(ip_str, " ");
		}
		//printf("Site: %s : %s\n", site , inet_ntoa(in));

		

	
	    ret = spidns_checkrobots( url , strlen(url) , resinfo);

		if(  ret == ROBOTS_RULE_FORBIDEN )
		{
			printf( "%s %s FORBIDEN\n" , url ,ip_str );
		}
		else
		{	
			printf( "%s %s GRANTED\n" , url ,ip_str );
		}	
		
    }

	spidns_destroy( pdns );

	fclose( fp );

	return 0;
}

#endif

#ifdef __TEST_ROBOTS
inline
int 
line_parse( char *robots_str , siteinfo_t &siteinfo  )
{

	char line[4096];
	char *start = NULL;
	char *end = NULL;
	char sitename[128];


	snprintf( line , 4096 , "dummy.site.com 127.0.0.1 A 1 1 %s\r\n" , robots_str);

	start = line;
	end = line + strlen( line );


	return  parse_resultline_singleip( start , end , sitename , 64 , siteinfo );

}





int main(int argc, char **argv)
{
        char line[1024];
		int ret;

		siteinfo_t sinfo;

        if (argc != 2) 
		{
                fprintf(stderr, "Usage:%s robots\n", argv[0]);
                return 0;
        }

		if ( 0 != line_parse( argv[1], sinfo) )
		{
				fprintf(stderr, "Cannot Parse robots:%s\n" , argv[1] );
				return 1;
		}


        while(fgets(line,1024,stdin)!=NULL)
		{
                line[strlen(line)-1] = '\0';
				ret = spidns_checkrobots( line , strlen(line) , sinfo);

                if(ret == ROBOTS_RULE_FORBIDEN )
				{
                        fprintf(stdout,"%s Forbidden\n",line);
                }
				else
				{
                        fprintf(stdout,"%s Allowed\n",line);
				}
        }

        return 0;
}
#endif



#ifdef TEST_SPIDNS_UDP

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void print_usage()
{
	cerr << "Usage: test_spidns_udp [-h] [-p port] [-f url_file] [-t timeout] [-i send_interval] dns_server" << endl;
}

int test_spidns_udp(sockaddr_in& serv_addr, istream& istr, int timeout, int64_t send_interval);

int main(int argc, char* argv[])
{
	string url_file;
	string serv_name;
	in_port_t port = 1280;
	int timeout = 10000;			// 超时值(单位毫秒)，默认为10秒
	int send_interval = 0;			// 发送间隔

	for (int i = 1; i < argc; ++i)
	{
		string arg = argv[i];
		if (arg == "-h")
		{
			print_usage();
			return 0;
		}
		else if (arg == "-p")
		{
			if (++i >= argc)
			{
				ul_writelog(UL_LOG_FATAL, "Missing port number.");
				return 1;
			}
			port = atoi(argv[i]);
		}
		else if (arg == "-f")
		{
			if (++i >= argc)
			{
				ul_writelog(UL_LOG_FATAL, "Missing URL file.");
				return 1;
			}
			url_file = argv[i];
		}
		else if (arg == "-t")
		{
			if (++i >= argc)
			{
				ul_writelog(UL_LOG_FATAL, "Missing timeout.");
				return 1;
			}
			timeout = atoi(argv[i]);
		}
		else if (arg == "-i")
		{
			if (++i >= argc)
			{
				ul_writelog(UL_LOG_FATAL, "Missing send interval.");
				return 1;
			}
			send_interval = atoi(argv[i]);
		}
		else
		{
			serv_name = arg;
		}
	}
	if (serv_name.empty())
	{
		ul_writelog(UL_LOG_FATAL, "Missing DNS server.");
		print_usage();
		return 1;
	}
	sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	if (ul_gethostipbyname_r(serv_name.c_str(), &serv_addr, BD_NET, NULL) != 0)
	{
		ul_writelog(UL_LOG_FATAL, "Resolve DNS server name (%s) fail.", serv_name.c_str());
		return 2;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (url_file.empty())
	{
		test_spidns_udp(serv_addr, cin, timeout, send_interval * 1000LL);
	}
	else
	{
		ifstream fin(url_file.c_str());
		if (!fin)
		{
			ul_writelog(UL_LOG_FATAL, "Open url file %s fail", url_file.c_str());
			return 3;
		}
		test_spidns_udp(serv_addr, fin, timeout, send_interval * 1000LL);
		fin.close();
	}
}

int test_spidns_udp(sockaddr_in& serv_addr, istream& istr, int timeout, int64_t send_interval)
{
	char sitename[DNS_MAX_SITE_LEN];
	siteinfo_t siteinfo;
	init_siteinfo(siteinfo);

	int64_t now = get_cur_time();
	int64_t start_time = now;
	int64_t total_url_count = 0;
	int64_t resolve_success_count = 0;
	int64_t robots_allow_count = 0;
	int64_t robots_disallow_count = 0;
	int64_t robots_error_count = 0;
	int64_t delay_sum = 0;			// DNS解析延时


	string url;
	getline(istr, url);
	for (; istr; getline(istr, url))
	{
		if (url.empty())
		{
			continue;
		}
		if (ul_get_site_ex(url.c_str(), sitename, DNS_MAX_SITE_LEN) == NULL)
		{
			cout << "Parse URL fail." << endl;
			continue;
		}
		
		now = get_cur_time();
		// flow control
		if (send_interval > 0)
		{
			int64_t expected_time = start_time + total_url_count * send_interval;
			if (now < expected_time)
			{
				usleep(expected_time - now);
				now = get_cur_time();
			}
		}
		++total_url_count;

		if (spidns_seeksite_udp((sockaddr*)&serv_addr, sizeof(serv_addr), sitename, siteinfo, timeout) == -1
				|| siteinfo.ip_num == 0)
		{
			cout << "DNS FAIL: " << url << endl;
			continue;
		}
		delay_sum += get_cur_time() - now;		// 计算解析延时
		++resolve_success_count;

		struct in_addr addr;
		addr.s_addr = siteinfo.ip[0];
		cout << "DNS OK. IP:" << inet_ntoa(addr) << ". ";
		cout << "robots_count: " << siteinfo.robots_data_num << ". ";
		int robots_rule = spidns_checkrobots(url.c_str(), url.size(), siteinfo);
		switch (robots_rule)
		{
			case ROBOTS_RULE_GRANTED:
				++ robots_allow_count;
				cout << "ROBOTS ALLOW: ";
				break;
			case ROBOTS_RULE_FORBIDEN:
				++ robots_disallow_count;
				cout << "ROBOTS DISALOW: ";
				break;
			case ROBOTS_RULE_ERROR:
				++ robots_error_count;
				cout << "ROBOTS ERROR: ";
				break;
			default:
				assert(false);
		}
		cout << url << endl;
	}
	clear_siteinfo(siteinfo);
	cerr << "--------------------------------------" << endl;
	cerr << "Total URL count           : " << total_url_count << endl;
	cerr << "Resolve success count     : " << resolve_success_count << endl;
	cerr << "Robots allow count        : " << robots_allow_count << endl;
	cerr << "Robots disallow count     : " << robots_disallow_count << endl;
	cerr << "Robots error count        : " << robots_error_count << endl;
	cerr << "Total time used           : " << (get_cur_time() - start_time) / 1000.0 << " ms" << endl;
	cerr << "Average DNS resovle delay : " << delay_sum / 1000.0 / total_url_count << " ms" << endl;
	return 0;
}

#endif // TEST_SPIDNS_UDP


/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
