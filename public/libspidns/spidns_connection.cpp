/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: spidns_connection.cpp,v 1.3 2008/09/25 07:33:46 jiangjinpeng Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file spidns_connection.cpp
 * @author chentj(spider@baidu.com)
 * @date 2008/03/26 12:09:14
 * @version $Revision: 1.3 $ 
 * @brief 
 *  
 **/
#include <sys/socket.h>
#include <netinet/tcp.h>
#include "ul_pack.h"
#include "ul_net.h"
#include "ul_log.h"
#include "spidns_connection.h" 
#include "spidns.h"

int
init_connection( dns_conn_t *pdns_conn , const char *dns_host , int dns_port , const char * localname , 
				 int retry_count )
{
	if ( pdns_conn == NULL )
	{
		return -1;
	}
	strcpy(pdns_conn->hostname , dns_host );
	strcpy(pdns_conn->localname , localname );
	pdns_conn->port = dns_port ;
	pdns_conn->fd = -1 ;
	pdns_conn->retry_count = retry_count;
	return 0;	
}


//return value:  0  success
//				-1	 分配内存失败   
//				-2   连接dns服务器失败
int 
connect_dns( dns_conn_t *pdns_conn )
{
	const char* where = "connect_dns";

	const int on = 1;
	int ret	=0;
	char result[10];

	char buf[1];

	ul_package *w_pack = NULL;
	ul_package *r_pack = NULL;
	char hostname[ 256 ];
	int	 retry_count = 0;	//失败重试计数，包括内存分配失败或连接dns服务失败
	

	if(pdns_conn == NULL)
	{
		return -1;
	}

	// init data pack
	retry_count = 0;
	while((w_pack = ul_pack_init()) == NULL)
	{
		ul_writelog(UL_LOG_WARNING, "[%s]: can not malloc for write_pack,retryed %d times\n",
					where, retry_count);
		
		retry_count++;
		if( pdns_conn->retry_count > 0 && retry_count >= pdns_conn->retry_count )
		{//查询方式为有限次重试，并且重试次数达到上限
			ul_writelog(UL_LOG_WARNING, "[%s]: can not malloc for write_pack,retryed %d times\n",
					where, retry_count);
			ret = -1;
			goto RETURN;
		}
		sleep(1);
	}

	retry_count = 0;
	while((r_pack = ul_pack_init()) == NULL)
	{
		ul_writelog(UL_LOG_WARNING, "[%s]: can not malloc for read_pack,retryed %d times\n", 
					where, retry_count);
		
		retry_count++;
        if( pdns_conn->retry_count > 0 && retry_count >=  pdns_conn->retry_count )
        {//查询方式为有限次重试，并且重试次数达到上限
            ul_writelog(UL_LOG_WARNING, "[%s]: can not malloc for read_pack,retryed %d times\n",
                    where, retry_count);
            ret = -1;
			goto RETURN;
        }
		sleep(1);
	}

	ul_pack_putbuf(r_pack, buf, 0);
	ul_pack_putbuf(w_pack, buf, 0);

	snprintf(hostname, 256 , "%s", pdns_conn->localname);
	ul_pack_putvalue(w_pack, "Sender", "Link-Selecter");
	ul_pack_putvalue(w_pack, "Protocol", "LOGIN");
	ul_pack_putvalue(w_pack, "Hostname", hostname);

	while(1)
	{
			// connect to dns server until success
		if( pdns_conn->fd == -1)
		{
			retry_count = 0;
			while(( pdns_conn->fd = ul_tcpconnect( pdns_conn->hostname, pdns_conn->port)) < 0 )
			{
				ul_writelog(UL_LOG_WARNING, "[%s]: connect to DNS error,retrying %d times\n",
							where, retry_count);
				
				retry_count++;
				if(pdns_conn->retry_count > 0 && retry_count >= pdns_conn->retry_count )
				{//有限次连接模式，并且连接次数达到上限
					ul_writelog(UL_LOG_WARNING, "[%s]: connect to DNS error, failed to many times!",
								where);
					ret = -2;
					goto RETURN;
					
				}
				sleep(5);
			}

			if(setsockopt(pdns_conn->fd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on)) < 0)
			{
				ul_writelog(UL_LOG_WARNING, "setsockopt error\n");
			}
		}

		ret = ul_pack_write(pdns_conn->fd, w_pack);

		if(ret < 0)
		{
			ul_writelog(UL_LOG_WARNING, "[%s]: write login pack to DNS error,retrying...\n", where);

			ul_close(pdns_conn->fd);
			pdns_conn->fd = -1;
			continue;
		}

		ret = ul_pack_reado(pdns_conn->fd, r_pack, 180);

		if(ret < 0)
		{
			ul_writelog(UL_LOG_WARNING, "[%s]: read login answer from DNS error,retrying...\n", where);

			ul_close(pdns_conn->fd);
			pdns_conn->fd = -1;

		   continue;
		}

		ul_pack_getvalue(r_pack, "Result", result);

		if(strcmp(result,"-1") == 0)
		{
			pdns_conn->fd = -1;
			ul_writelog(UL_LOG_WARNING, "[%s]: can not login DNS,retrying...\n", where);
			continue;
		}
		
		ret = 0;
		break;
	}

RETURN:
	if(w_pack != NULL )
		ul_pack_free(w_pack);
	if(r_pack != NULL )
		ul_pack_free(r_pack);
	return ret;
}

int 
write_dns( dns_conn_t *pdns_conn , dns_pack_t *ppack)
{
	int ret = 0;
	if ( pdns_conn == NULL || ppack == NULL || pdns_conn->fd < 0 )
	{
		return -1;
	}
    ret = ul_pack_write( pdns_conn->fd , ppack->upack );
	return ret;
}

int 
read_dns( dns_conn_t *pdns_conn , dns_pack_t *ppack)
{
    int ret = 0;
    if ( pdns_conn == NULL || ppack == NULL || pdns_conn->fd < 0 )
    {
        return -1;
    }
    ret = ul_pack_reado( pdns_conn->fd , ppack->upack , 180 );
    return ret;
}

int 
build_dnspack_head( dns_pack_t *ppack, int ip_mode )
{
	if( ppack == NULL || ppack->upack == NULL )
	{
		return -1;
	}

	ul_pack_putvalue( ppack->upack , "Sender" ,"Link-Selecter");

	switch(ip_mode)
	{
		case IP_SINGLE:
			ul_pack_putvalue( ppack->upack , "Version" , "1.0");
			break;
		case IP_MULTI:
			ul_pack_putvalue( ppack->upack, "Version", "2.0");
			break;
		default:
			ul_writelog(UL_LOG_WARNING, "[%s]: error ip mode[%d]",__FUNCTION__, ip_mode);
			assert(0);
	}
	return 0;
}

int 
push_dnspack_body( dns_pack_t *ppack , char *buf , int len , int num )
{
	char numbuf[16];

    if( ppack == NULL || ppack->upack == NULL )
    {
        return -1;
    }
	
	snprintf( numbuf , 15 , "%d", num );
	
	ul_pack_putvalue( ppack->upack , "Number" , numbuf );
	
	ul_pack_putbuf( ppack->upack ,  buf , len );
	return 0;
}

int 
get_dnspack_number( dns_pack_t *ppack )
{
	char tmp_str[32];
    
    if( 1 != ul_pack_getvalue(ppack->upack, "Number", tmp_str))
	{
		return -1;
	}
	
	return atoi(tmp_str);
}

dns_pack_t* 
init_dnspack()
{
	dns_pack_t* ppack = NULL;
	ppack = new dns_pack_t;
	if( ppack == NULL )
	{
		return NULL;
	}

	ppack->upack = ul_pack_init();

	if ( ppack->upack == NULL )
	{
		delete ppack ;
		return NULL;
	}

	return ppack;
}

void 
destroy_dnspack( dns_pack_t *ppack )
{
	if( ppack == NULL || ppack->upack == NULL )
	{
		return ;
	}

	free( ppack->upack );

	delete ppack;

	return;
}



void 
close_connection( dns_conn_t *pdns_conn )
{
	if( pdns_conn->fd > 0 )
	{
		ul_close( pdns_conn->fd );
	}	
	pdns_conn->fd = -1;
	
}



/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
