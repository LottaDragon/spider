/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: mc_pack_factory.h,v 1.1.2.2 2008/12/08 11:03:30 yufan Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file mc_pack_factory.h
 * @author yufan(com@baidu.com)
 * @date 2008/11/25 14:43:19
 * @version $Revision: 1.1.2.2 $ 
 * @brief 
 *  
 **/


#ifndef  __MC_PACK_FACTORY_H_
#define  __MC_PACK_FACTORY_H_


#include "mc_pack1.h"
#include "mc_pack2.h"

class mc_pack_factory
{
public:
	static int create(int version, char * buf, int size, char * tempbuf, int tem_size, Mcpack ** ppack);
	static int read(char * buf, int size, char * tempbuf, int tem_size, Mcpack ** ppack);
	static int reopen(char * buf, int size, char * tempbuf, int tem_size, Mcpack ** ppack);
};


template <typename alloc_t>
class mc_pack_factory_t
{
	public:
	static int create(int version, char * buf, int size, alloc_t * alloc, Mcpack ** ppack)
	{
		if(version == 1)
		{
			return Protocol_v1<alloc_t>::create(buf, size, alloc, (Protocol_v1<alloc_t>**)ppack);
		}
		else
		{
			return Protocol_v2<alloc_t>::create(buf, size, alloc, (Protocol_v2<alloc_t>**)ppack);
		}
	}

	static int read(char * buf, int size, alloc_t * alloc, Mcpack ** ppack)
	{
		int tag = *(int*)buf;
		if(tag == *(int *)"PCK")
		{
			return Protocol_v1<alloc_t>::create(buf, size, alloc, (Protocol_v1<alloc_t>**)ppack);
		}
		else
		{
			return Protocol_v2<alloc_t>::create(buf, size, alloc, (Protocol_v2<alloc_t>**)ppack);
		}

	}
	static int reopen(char * buf, int size, alloc_t * alloc, Mcpack ** ppack)
	{
		int tag = *(int*)buf;
		if(tag == *(int *)"PCK")
		{
			return Protocol_v1<alloc_t>::create(buf, size, alloc,(Protocol_v1<alloc_t>**)ppack);
		}
		else
		{
			return Protocol_v2<alloc_t>::create(buf, size, alloc, (Protocol_v1<alloc_t>**)ppack);
		}

	}

};

#endif  //__MC_PACK_FACTORY_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
