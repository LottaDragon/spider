/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: Int32.h,v 1.4 2009/06/26 07:24:14 zhangfang Exp $ 
 * 
 **************************************************************************/



/**
 * @file Int32.h
 * @author chenxm(chenxiaoming@baidu.com)
 * @date 2008/09/24 01:32:42
 * @version $Revision: 1.4 $ 
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_INT32_H__
#define  __BSL_VAR_INT32_H__

#include <cstdlib>
#include <climits>
#include "bsl/var/IVar.h"
#include "bsl/var/Ref.h"
#include "bsl/check_cast.h"

namespace bsl{
namespace var{

    class Int32: public IVar{
    public:
        typedef IVar::string_type   string_type;
        typedef IVar::field_type    field_type;

    public:
        //special methods
        Int32( int __value = 0 ) 
            :_value(__value){}

        Int32( const Int32& other )
            :IVar(other), _value( other._value ) {}

        Int32& operator = ( const Int32& other ){
            _value = other._value;
            return *this;
        }

        //methods for all
        virtual Int32& operator = ( IVar& var ) {
            try{
                _value = var.to_int32();    //throw
            }catch(bsl::Exception& e){
                e<<"{bsl::var::Int32::operator =("<<var.dump()<<")}";
                throw;
            }
            return *this;
        }

        virtual void clear(){
            _value = 0;
        }

        virtual string_type dump(size_t /*verbose_level*/ = 0) const {
            return  string_type("[bsl::var::Int32]").appendf("%d", _value);
        }

        virtual string_type to_string() const {
            return  string_type().appendf("%d", _value);
        }

        virtual string_type get_type() const {
            return "bsl::var::Int32";
        }

        virtual bool to_bool() const {
            return _value;
        }

        virtual int to_int32() const {
            return _value;
        }

        virtual long long to_int64() const {
            return check_cast<long long>(_value);
        }

        virtual double to_double() const {
            return check_cast<double>(_value);
        }

        virtual Int32& operator = ( bool b ){
            _value = b;
            return *this;
        }

        virtual Int32& operator = ( int i ){
            _value = i;
            return *this;
        }

        virtual Int32& operator = ( long long ll ){
            _value = check_cast<int>(ll);
            return *this;
        }

        virtual Int32& operator = ( double d ){
            _value = check_cast<int>(d);
            return *this;
        }

        virtual Int32& operator = ( const char *cstr ){
            _value = check_cast<int>(cstr);
            return *this;
        }

        virtual Int32& operator = ( const string_type& str ){
            _value = check_cast<int>(str.c_str());
            return *this;
        }

        //use default version for raw
        using IVar::operator =;

        //testers
        virtual bool is_number() const {
            return true;
        }

        virtual bool is_int32() const {
            return true;
        }

    private:
        int _value;
    };

}}   //namespace bsl::var

#endif  //__BSL_VAR_INT32_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
