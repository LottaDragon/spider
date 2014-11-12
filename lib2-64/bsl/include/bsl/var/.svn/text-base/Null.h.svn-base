/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: Null.h,v 1.4 2009/06/26 07:24:14 zhangfang Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file Null.h
 * @author chenxm(chenxiaoming@baidu.com)
 * @date 2008/09/24 20:53:41
 * @version $Revision: 1.4 $ 
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_NULL_H__
#define  __BSL_VAR_NULL_H__
#include "bsl/var/IVar.h"

namespace bsl{
namespace var{
    class Null: public IVar{
    public:
        typedef IVar::string_type          string_type;
        typedef IVar::field_type           field_type;

        //special methods
        Null(){ }

        Null( const Null& other ):IVar(other){ }

        virtual ~Null(){ } 

        Null& operator = ( const Null& ){
            return *this;
        }

        //methods for all
        virtual void clear(){
            // pass
        }

        virtual string_type dump(size_t /*verbose_level*/ = 0) const {
            return "[bsl::var::Null]null";
        }

        virtual string_type to_string() const {
            return "null";
        }

        virtual string_type get_type() const {
            return "bsl::var::Null";
        }

        virtual Null& operator = ( IVar& var ){
            if ( !var.is_null() ){
                throw bsl::InvalidOperationException()<<BSL_EARG<<"{"<<__PRETTY_FUNCTION__<<"("<<var.dump()<<")}";
            }
            return *this;
        }

        virtual bool is_null() const {
            return true;
        }

        using IVar::operator =;

    public:
        static Null null;
    };

}}   //namespace bsl::var
#endif  //__BSL_VAR_NULL_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
