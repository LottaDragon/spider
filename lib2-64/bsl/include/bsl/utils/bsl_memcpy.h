/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: bsl_memcpy.h,v 1.5 2009/06/26 07:24:14 zhangfang Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file xmemcpy.h
 * @author xiaowei(com@baidu.com)
 * @date 2008/11/07 17:17:22
 * @version $Revision: 1.5 $ 
 * @brief 
 *  
 **/


#ifndef  __XMEMCPY_H_
#define  __XMEMCPY_H_

#include <string.h>

namespace bsl
{
static const size_t XMEM_MAXSIZE = 120;

class xmembase
{
public:
	virtual void * copy (void *dest, const void *src) = 0;
};

extern xmembase * g_base[XMEM_MAXSIZE];

inline void *xmemcpy (void *dest, const void *src, size_t len)
{
	//·ÀÖ¹ÖØµþ
	long dst = ((long)dest - (long)src);
	if ( (dst < (long)len && dst > 0 - (long)len) ||
			len >= XMEM_MAXSIZE) {
		return ::memcpy(dest, src, len);
	}

	return g_base[len]->copy(dest, src);
}

};



#endif  //__XMEMCPY_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
