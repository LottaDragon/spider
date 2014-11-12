/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: ShallowString.h,v 1.2 2009/06/26 07:24:14 zhangfang Exp $ 
 * 
 **************************************************************************/



/**
 * @file ShallowString.h
 * @author chenxm(chenxiaoming@baidu.com)
 * @date 2008/09/24 01:34:12
 * @version $Revision: 1.2 $ 
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_SHALLOW_STRING_H__
#define  __BSL_VAR_SHALLOW_STRING_H__
#include "bsl/var/IVar.h"
#include "bsl/var/Ref.h"
#include "bsl/check_cast.h"
#include "bsl/ShallowCopyString.h"

namespace bsl{
namespace var{
    class ShallowString: public IVar{
    public:
        typedef IVar::string_type   string_type;
        typedef IVar::field_type    field_type;

    public:
        //special methods
        ShallowString( )
            : _value() {}

        ShallowString( const ShallowString& other )
            : IVar(other), _value(other._value) { }

        ShallowString( const char * cstr )
            : IVar(), _value(cstr) { }

        ShallowString( const char * cstr, size_t len )
            : IVar(), _value(cstr, len) { }

        ShallowString( const bsl::ShallowCopyString& value_)
            : IVar(), _value( value_.c_str(), value_.size() ) { }


        ShallowString( const string_type& value_ )
            : IVar(), _value(value_.c_str(), value_.size()){}

        ShallowString& operator = ( const ShallowString& other ){
            _value = other._value;
            return *this;
        }

        virtual ~ShallowString(){
            //pass
        }

        //methods for all
        virtual ShallowString& operator = ( IVar& var ){
            if ( var.is_string() ){
                _value = var.c_str();
                return *this;
            }else{
                throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<_value.c_str()<<"] cast from["<<var.dump(0)<<"]";
            }
        }

        virtual string_type dump(size_t /*verbose_level*/=0) const {
            return  string_type("[bsl::var::ShallowString]").append(_value.c_str(), _value.size());
        }

        virtual string_type to_string() const {
            return _value.c_str();
        }

        virtual string_type get_type() const {
            return "bsl::var::ShallowString";
        }

        virtual void clear(){
            _value.clear();
        }

        //testers
        virtual bool is_string() const {
            return true;
        }

        //methods for value
        virtual bool to_bool() const {
            return _value.c_str()[0] != '\0';
        }

        virtual int to_int32() const {
            return check_cast<int>(_value.c_str());
        }

        virtual long long to_int64() const {
            return check_cast<long long>(_value.c_str());
        }

        virtual double to_double() const {
            return check_cast<double>(_value.c_str());
        }

        virtual ShallowString& operator = ( const char * cstr ){
            _value = cstr;
            return *this;
        }

        virtual ShallowString& operator = ( const string_type& str ){
            _value = str;
            return *this;
        }

        //use default version for bool、raw、number
        using IVar::operator =;

        /**
         * @brief 返回C风格字符串表示
         *
         * 所有is_string()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * @return  const char* 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/04/22 18:41:19
        **/
        virtual const char * c_str() const {
            return _value.c_str();
        }

        /**
         * @brief 返回C风格字符串表示的长度，不包括末尾的'\0'
         *
         * 所有is_string()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * @return  size_t 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/04/22 18:41:54
        **/
        virtual size_t c_str_len() const {
            return _value.size();
        }

    private:
        bsl::ShallowCopyString _value;
    };

}}//namespace bsl::var

#endif  //__BSL_VAR_SHALLOW_STRING_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
