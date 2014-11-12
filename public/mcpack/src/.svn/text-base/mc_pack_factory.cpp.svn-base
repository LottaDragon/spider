/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: mc_pack_factory.cpp,v 1.1.2.3 2008/12/08 11:03:30 yufan Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file mc_pack_factory.cpp
 * @author yufan(com@baidu.com)
 * @date 2008/11/25 14:47:24
 * @version $Revision: 1.1.2.3 $ 
 * @brief 
 *  
 **/
#include "mc_pack_bits/mc_pack_factory.h"
#include "mc_pack_bits/allocator.h"

int mc_pack_factory::create(int version, char * buf, int size, char * tempbuf, int tmp_size, Mcpack ** ppack)
{
	int tag = *(int*)buf;
	int result;
	if(tag != *(int *)"PCK")
	{
		result =  Protocol_v2<reverse_allocator>::reopen(buf, size, tempbuf, tmp_size, (Protocol_v2<reverse_allocator> **)ppack);
	}
	else
	{
		result =  Protocol_v1<reverse_allocator>::reopen(buf, size, tempbuf, tmp_size, (Protocol_v1<reverse_allocator> **)ppack);
	}
	return 0;
}

int mc_pack_factory::read(char * buf, int size, char * tempbuf, int tem_size, Mcpack ** ppack)
{
	int tag = *(int*)buf;
	if(tag == *(int *)"PCK")
	{
		return Protocol_v1<reverse_allocator>::read(buf, size, tempbuf, tem_size, (Protocol_v1<reverse_allocator> **)ppack);
	}
	else
	{
		return Protocol_v2<reverse_allocator>::read(buf, size, tempbuf, tem_size, (Protocol_v2<reverse_allocator> **)ppack);
	}

}

int mc_pack_factory::reopen(char * buf, int size, char * tempbuf, int tem_size, Mcpack ** ppack)
{
	int tag = *(int*)buf;
	if(tag == *(int *)"PCK")
	{
		return Protocol_v1<reverse_allocator>::reopen(buf, size, tempbuf, tem_size, (Protocol_v1<reverse_allocator> **)ppack);
	}
	else
	{
		return Protocol_v2<reverse_allocator>::reopen(buf, size, tempbuf, tem_size, (Protocol_v2<reverse_allocator> **)ppack);
	}

}















/* vim: set ts=4 sw=4 sts=4 tw=100 */
