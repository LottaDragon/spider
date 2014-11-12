/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: Bool.h,v 1.2 2009/06/26 07:24:14 zhangfang Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file Bool.h
 * @author chenxm(chenxiaoming@baidu.com)
 * @date 2008/09/24 20:53:41
 * @version $Revision: 1.2 $ 
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_BOOL_H__
#define  __BSL_VAR_BOOL_H__
#include "bsl/var/IVar.h"

namespace bsl{
namespace var{
    class Bool: public IVar{
    public:
        typedef IVar::string_type           string_type;
        typedef IVar::field_type            field_type;

        //special methods
        Bool()
            :_value(false){}

        Bool( const Bool& other )
            :IVar(other), _value(other._value){ }

        explicit Bool( bool value_ )
            :IVar(), _value(value_){}

        virtual ~Bool(){ } 

        Bool& operator = ( const Bool& other ){
            _value = other._value;
            return *this;
        }

        //methods for all
        virtual void clear(){
            _value = false;
        }

        virtual string_type dump(size_t /*verbose_level*/ = 0) const {
            if ( _value ){
                return "[bsl::var::Bool]true";
            }else{
                return "[bsl::var::Bool]false";
            }
        }

        virtual string_type to_string() const {
            if ( _value ){
                return "true";
            }else{
                return "false";
            }
        }

        virtual string_type get_type() const {
            return "bsl::var::Bool";
        }

        virtual Bool& operator = ( IVar& other ){
            _value = other.to_bool();
            return *this;
        }

        virtual bool is_bool() const {
            return true;
        }

        //all other is_xxx() return false;

        //methods for bool
        virtual Bool& operator = ( bool val ){
            _value = val;
            return *this;
        }

        virtual Bool& operator = ( int val ){
            _value = val;
            return *this;
        }

        virtual Bool& operator = ( long long val ){
            _value = val;
            return *this;
        }

        virtual Bool& operator = ( double val ){
            _value = val;
            return *this;
        }

        virtual Bool& operator = ( const char * val ){
            _value = (NULL != val && '\0' != val[0]);
            return *this;
        }

        virtual Bool& operator = ( const string_type& val ){
            _value = (val.c_str()[0] != '\0') ;
            return *this;
        }

        virtual bool to_bool() const {
            return _value;
        }

        virtual int to_int32() const {
            return _value;
        }

        virtual long long to_int64() const {
            return _value;
        }

        virtual double to_double() const {
            return _value;
        }

        //using default version for raw
        using IVar::operator =;
    private:
        bool _value;
    };

}}   //namespace bsl::var
#endif  //__BSL_VAR_BOOL_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
