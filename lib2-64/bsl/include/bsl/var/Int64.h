/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: Int64.h,v 1.4 2009/06/26 07:24:14 zhangfang Exp $ 
 * 
 **************************************************************************/



/**
 * @file Int64.h
 * @author chenxm(chenxiaoming@baidu.com)
 * @date 2008/09/24 01:32:42
 * @version $Revision: 1.4 $ 
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_INT64_H__
#define  __BSL_VAR_INT64_H__

#include <cstdlib>
#include <climits>
#include "bsl/var/IVar.h"
#include "bsl/var/Ref.h"
#include "bsl/check_cast.h"

namespace bsl{
namespace var{
    class Int64: public IVar{
    public:
        typedef IVar::string_type   string_type;
        typedef IVar::field_type    field_type;

    public:
        //special methods
        Int64( long long __value = 0 ) :_value(__value){}

        Int64( const Int64& other )
            :IVar(other), _value( other._value ) {}

        Int64& operator = ( const Int64& other ){
            _value = other._value;
            return *this;
        }

        //methods for all
        virtual Int64& operator = ( IVar& var ) {
            try{
                _value = var.to_int64();    //throw
            }catch(bsl::Exception& e){
                e<<"{"<<__PRETTY_FUNCTION__<<"("<<var.dump()<<")}";
                throw;
            }
            return *this;
        }

        virtual void clear(){
            _value = 0;
        }

        
        virtual string_type dump(size_t /*verbose_level*/ = 0) const {
            return  string_type("[bsl::var::Int64]").appendf("%lld", _value);
        }

        virtual string_type to_string() const {
            return  string_type().appendf("%lld", _value);
        }

        virtual string_type get_type() const {
            return "bsl::var::Int64";
        }

        virtual bool to_bool() const {
            return _value;
        }

        virtual int to_int32() const {
            return check_cast<int>(_value);
        }

        virtual long long to_int64() const {
            return _value;
        }

        virtual double to_double() const {
            return check_cast<double>(_value);
        }

        virtual Int64& operator = ( bool b ){
            _value = b;
            return *this;
        }

        virtual Int64& operator = ( int i ){
            _value = check_cast<long long>(i);
            return *this;
        }

        virtual Int64& operator = ( long long ll ){
            _value = ll;
            return *this;
        }

        virtual Int64& operator = ( double d ){
            _value = check_cast<long long>(d);
            return *this;
        }

        virtual Int64& operator = ( const char * cstr ){
            _value = check_cast<long long>(cstr);
            return *this;
        }

        virtual Int64& operator = ( const string_type& str ){
            _value = check_cast<long long>(str.c_str());
            return *this;
        }

        //use default version for bool¡¢raw
        using IVar::operator =;

        //testers
        virtual bool is_number() const {
            return true;
        }

        virtual bool is_int64() const {
            return true;
        }

    private:
        long long _value;
    };

}}   //namespace bsl::var

#endif  //__BSL_VAR_INT64_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
