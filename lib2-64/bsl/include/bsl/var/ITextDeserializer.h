/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: ITextDeserializer.h,v 1.4 2009/06/26 07:24:14 zhangfang Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file ITextDeserializer.h
 * @author chenxm(chenxiaoming@baidu.com)
 * @date 2008/10/07 21:16:20
 * @version $Revision: 1.4 $ 
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_ITEXT_DESERIALIZER_H__
#define  __BSL_VAR_ITEXT_DESERIALIZER_H__

#include "bsl/var/IVar.h"
#include "bsl/var/Ref.h"
namespace bsl{
namespace var{

    class ITextDeserializer{
    public:
        virtual ~ITextDeserializer(){
        }
        virtual IVar& deserialize(const char * text) = 0;
    };

}}   //namespace bsl::var

#endif  //__BSL_VAR_ITEXT_DESERIALIZER_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
