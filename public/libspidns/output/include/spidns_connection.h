/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: spidns_connection.h,v 1.2 2008/09/25 04:23:18 jiangjinpeng Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file spidns_connection.h
 * @author chentj(spider@baidu.com)
 * @date 2008/03/26 12:08:56
 * @version $Revision: 1.2 $ 
 * @brief 
 *  
 **/
#ifndef SPIDNS_CONNECTION_H
#define SPIDNS_CONNECTION_H

#include "ul_pack.h"

typedef struct _dns_connection
{
	int 	fd ;
	char 	hostname[ 256 ];
	int 	port ;
	char 	localname[ 256 ];
	int		retry_count;
}dns_conn_t;

typedef struct _dns_pack
{
	ul_package *upack;
}dns_pack_t;


int init_connection( dns_conn_t *pdns_conn, const char *dns_host , int dns_port , const char *localname, 
					 int retry_count = 0);

int connect_dns( dns_conn_t *pdns_conn );

int write_dns( dns_conn_t *pdns_conn , dns_pack_t *ppack);

int read_dns( dns_conn_t *pdns_conn , dns_pack_t *ppack);

int build_dnspack_head( dns_pack_t *ppack,  int ip_mode );

int push_dnspack_body( dns_pack_t *ppack , char *buf , int len , int num );

int get_dnspack_number( dns_pack_t *ppack );

dns_pack_t* init_dnspack();

void destroy_dnspack( dns_pack_t *ppack );

void close_connection( dns_conn_t *pdns_conn );


#endif
/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
