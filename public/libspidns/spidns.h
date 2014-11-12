/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: spidns.h,v 1.3 2008/10/08 02:56:27 jiangjinpeng Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file spidns.h
 * @author chentj(spider@baidu.com)
 * @date 2008/03/05 11:40:40
 * @version $Revision: 1.3 $ 
 * @brief 
 *  
 **/
#ifndef SPIDNS_H
#define SPIDNS_H

#include "spidns_connection.h" 
#include "spidns_data.h"


#define	NO_SYNC			0x0010  //DO NOT sync , debugging stuff
#define MANUAL_SYNC		0x0020  //sync with spi-dns when sync function is called
#define AUTO_SYNC		0x0040  //Automatic Sync with spi-dns , NOT IMPLEMENTED YET!

#define ROBOTS_NONE		0x0100  //DO NOT process ANY robots data
#define ROBOTS_REGEXP	0x0200  //process robots data with regular expr.
#define ROBOTS_TEXTONLY 0x0400  //process robots data as plain-text only

#define	IP_SINGLE		0x0001  //单ip查询，默认情况下采用单ip查询
#define IP_MULTI		0x0002  //多ip查询

#define MAX_DNS_RESPONSE_TIME	1000000	// DNS服务器最大响应时间，单位微秒

typedef struct _dns_wrapper_t
{
	dns_conn_t mdns_conn;
	dns_data_t *pdns_data;
	int cache_size;
	dns_mode_t mdns_mode;

}spidns_t;


/**
 * @brief 初始化spidns_t 分配内存
 *
 * @param [in] cache_size   : int  缓存大小  1.0版本无用
 * @param [in] mode   : int 模式，参见模式定义,1.0版本只支持IP_SINGLE,IP_MULTI
 * @param [in] dns_host   : char*  dns服务地址
 * @param [in] dns_port   : int    dns端口
 * @param [in] local_name   : char*   本地客户端名称
 * @param [in] retry_count  : int	连接服务器失败的重试次数，0表示无限重试
 * @return  spidns_t*  指向新分配的spidns_t指针
 * @retval  成功则返回指针，失败就返回NULL
 * @see 
 * @note 
 * @author chentj
 * @date 2008/04/24 16:04:23
**/
spidns_t *spidns_init( int cache_size , int mode , const char *dns_host , int dns_port ,const char *local_name,
					   int retry_count = 0);


/**
 * @brief 释放spidns_t 内存
 *
 * @param [in] pdns   : spidns_t* 被释放的对象
 * @return  void 
 * @retval   
 * @see 
 * @note 
 * @author chentj
 * @date 2008/03/05 11:42:25
**/
void spidns_destroy( spidns_t* pdns );

/**
 * @brief 向待解析队列中插入一个站点
 *
 * @param [in] pdns   : spidns_t* spidns结构指针  
 * @param [in] sitename   : char* 站点名
 * @return  成功返回 0 失败返回 -1
 * @retval   
 * @see 
 * @note 
 * @author chentj
 * @date 2008/03/05 11:42:38
**/
int spidns_pushsite( spidns_t* pdns , const char *sitename , int site_len );


/**
 * @brief 向本地缓存强制 更新DNS信息
 *
 * @param [in/out] pdns   : spidns_t* 
 * @param [in/out] dnsinfo_line   : char*
 * @param [in/out] len   : int
 * @return  int 
 * @retval  成功返回0，失败返回-1 
 * @see 
 * @note 
 * @author chentj
 * @date 2008/04/21 14:28:11
**/
int spidns_updateline( spidns_t* pdns , char *dnsinfo_line , int len );

/**
 * @brief 在本地缓存中 检索DNS信息
 *
 * @param [in] pdns   : spidns_t*
 * @param [in] sitename   : char*  站点名
 * @param [out] pout   : siteinfo_t& 返回的站点信息
 * @return  int 成功 0 失败 -1
 * @retval   
 * @see 
 * @note 
 * @author chentj
 * @date 2008/03/05 11:42:56
**/

int spidns_seeksite( spidns_t* pdns , const char *sitename , int site_len ,  siteinfo_t &pout );


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
int spidns_seeksite_udp(const sockaddr* servaddr, socklen_t addrlen, const char* sitename
						, siteinfo_t& siteinfo, int timeout = 0);

/**
 * @brief 将待解析队列中的DNS数据与本地同步 
 *
 * @param [in/out] pdns   : spidns_t* spidns对象
 * @return  int 成功返回0 失败返回 -1
 * @retval   
 * @see 
 * @note 
 * @author chentj
 * @date 2008/03/05 11:42:59
**/
int spidns_syncdata( spidns_t* pdns );

/**
 * @brief 清除所有数据
 *
 * @param [in/out] pdns   : spidns_t* spidns_t结构
 * @return  int 
 * @retval  成功返回0，失败返回-1 
 * @see 
 * @note 
 * @author chentj
 * @date 2008/03/05 11:43:05
**/
int spidns_clear( spidns_t* pdns );

#define ROBOTS_RULE_FORBIDEN 1
#define ROBOTS_RULE_GRANTED  0
#define ROBOTS_RULE_ERROR   -1

/**
 *  * @brief 检查robots是否禁止
 *
 * @param [in] purl   :	待检测的url
 * @param [in] ulen	  : 待检测url的长度
 * @param [in] siteinfo : url对应的站点数据
 * @return  int 
 * @retval 参见 ROBOTS_RULE宏定义 
 * @see 
 * @note 
 * @author chentj
 * @date 2008/03/05 11:43:05
 **/
int spidns_checkrobots( const char *purl , int ulen , const siteinfo_t& siteinfo );

#endif
/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
