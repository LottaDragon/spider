/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: spidns_data.h,v 1.3 2008/10/08 02:55:22 jiangjinpeng Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file spidns_data.h
 * @author chentj(spider@baidu.com)
 * @date 2008/03/05 11:40:00
 * @version $Revision: 1.3 $ 
 * @brief 
 *  
 **/

#ifndef SPIDNS_DATA_H
#define SPIDNS_DATA_H

#include <map>
#include <vector>
#include <list>

using namespace std;

#include "spidns_robots.h"

#define DNS_MAX_SITE_LEN 72
#define DNS_MAX_PACK_NUM 100


typedef struct _site_infomation
{        
	robot_t *robots_data;
	int  robots_data_num;
	unsigned int *ip;	
	int	 ip_num;			//ip数量
	char ishttp11;			//是否支持http11，当前实现采用默认值1
    char ipregion[4];
	char casesense[4];
}siteinfo_t;

typedef struct _site_name_buffer
{
	unsigned char *buf;
	int  len;
	int  num;
}snbuf_t;

typedef struct _data_pool
{
	map< unsigned int , siteinfo_t > res_index;
	vector< snbuf_t > sitename_data;
	char sn_buf[ DNS_MAX_SITE_LEN*DNS_MAX_PACK_NUM ];
	int sn_len;
	int sn_num;
}dns_data_t;

typedef struct _dns_mode_t
{
	int	ip;
	int sync;
	int	robots;
}dns_mode_t;

dns_data_t * init_dnsdata( int cache_size );

int  init_dnsmode( int mode, dns_mode_t * dns_mode );

void init_siteinfo(siteinfo_t& siteinfo);

void clear_siteinfo(siteinfo_t& siteinfo);

void release_dnsdata( dns_data_t* pdns_data );

unsigned int get_sitesign(const char *sitename , int len );

int add_buffer( dns_data_t* pdns_data , const char *sitename , int site_len );

int clear_buffer( dns_data_t* pdns_data );

int pop_buffer( dns_data_t* pdns_data , char *buf , int *buflen );

#define SITEINFO_INCACHE	0
#define SITEINFO_NOTCACHE	1
#define SITEINFO_ERROR		-1


int add_siteinfo_cache( dns_data_t* pdns_data , unsigned int site_key , siteinfo_t siteinfo );

int update_siteinfo_cache( dns_data_t* pdns_data , unsigned int site_key , siteinfo_t siteinfo );

int seek_siteinfo( dns_data_t* pdns_data , unsigned int site_key , siteinfo_t &output_site );

int clear_cache( dns_data_t* pdns_data );

int parse_dnsdumpline( char *linestart , char *lineend , char *sitename , int buflen , siteinfo_t &output_site );

int parse_resultline_singleip( char *linestart , char *lineend , char *sitename , int buflen , siteinfo_t &output_site );
int parse_resultline_multiip( char *linestart , char *lineend , char *sitename , int buflen , siteinfo_t &output_site );
#endif
/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
