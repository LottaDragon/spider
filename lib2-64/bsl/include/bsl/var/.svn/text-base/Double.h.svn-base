/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: Double.h,v 1.4 2009/06/26 07:24:14 zhangfang Exp $ 
 * 
 **************************************************************************/



/**
 * @file Double.h
 * @author chenxm(chenxiaoming@baidu.com)
 * @date 2008/09/24 01:32:42
 * @version $Revision: 1.4 $ 
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_DOUBLE_H__
#define  __BSL_VAR_DOUBLE_H__

#include "bsl/var/IVar.h"
#include "bsl/var/Ref.h"
#include "bsl/check_cast.h"

namespace bsl{
namespace var{
    class Double: public IVar{
    public:
        typedef IVar::string_type   string_type;
        typedef IVar::field_type    field_type;

    public:
        //special methods
        Double( double __value = 0 ) :_value(__value){}

        Double( const Double& other )
            :IVar(other), _value( other._value ) {}

        Double& operator = ( const Double& other ){
            _value = other._value;
            return *this;
        }

        //methods for all
        virtual Double& operator = ( IVar& var ) {
            try{
                _value = var.to_double();    //throw
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
            return  string_type("[bsl::var::Double]").appendf("%lg", _value);
        }

        virtual string_type to_string() const {
            return  string_type().appendf("%lg", _value);
        }

        virtual string_type get_type() const {
            return "bsl::var::Double";
        }

        virtual bool to_bool() const {
            return _value;
        }

        virtual int to_int32() const {
            return check_cast<int>(_value);
        }

        virtual long long to_int64() const {
            return check_cast<long long>(_value);
        }

        virtual double to_double() const {
            return _value;
        }

        virtual Double& operator = ( bool b ){
            _value = b;
            return *this;
        }

        virtual Double& operator = ( int i ){
            _value = check_cast<double>(i);
            return *this;
        }

        virtual Double& operator = ( long long ll ){
            _value = check_cast<double>(ll);
            return *this;
        }

        virtual Double& operator = ( double d ){
            _value = d;
            return *this;
        }

        virtual Double& operator = ( const char * cstr ){
            _value = check_cast<double>(cstr);
            return *this;
        }

        virtual Double& operator = ( const string_type& str ){
            _value = check_cast<double>(str.c_str());
            return *this;
        }

        //use default version for bool¡¢raw
        using IVar::operator =;

        //testers
        virtual bool is_number() const {
            return true;
        }

        virtual bool is_double() const {
            return true;
        }

    private:
        double _value;
    };

}}   //namespace bsl::var

#endif  //__BSL_VAR_DOUBLE_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
