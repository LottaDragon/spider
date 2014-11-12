/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: IBinaryDeserializer.h,v 1.4 2009/06/26 07:24:14 zhangfang Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file IBinaryDeserializer.h
 * @author duchuanying(duchuanying@baidu.com)
 * @date 2008/11/01 21:37:08
 * @version $Revision: 1.4 $ 
 * @brief mcpack -> IVar
 *  
 **/


#ifndef  __BSL_VAR_IBINARY_DESERIALIZER_H__
#define  __BSL_VAR_IBINARY_DESERIALIZER_H__

#include "bsl/var/IVar.h"
namespace bsl{
namespace var{

class IBinaryDeserializer{
public:
    virtual ~IBinaryDeserializer(){
    }
    virtual IVar& deserialize(const void* buf, size_t max_size) = 0;
};
}}   //namespace bsl::var


#endif  //__BSL_VAR_IBINARY_DESERIALIZER_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
