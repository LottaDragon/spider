/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: IVar.h,v 1.4 2009/06/26 07:24:14 zhangfang Exp $ 
 * 
 **************************************************************************/



/**
 * @file IVar.h
 * @author chenxm(chenxiaoming@baidu.com)
 * @date 2008/09/24 01:29:15
 * @version $Revision: 1.4 $ 
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_IVAR_H__
#define  __BSL_VAR_IVAR_H__

#include "bsl/containers/string/bsl_string.h"
#include "bsl/exception/bsl_exception.h"
#include "bsl/ResourcePool.h"
#include "bsl/var/ArrayIterator.h"
#include "bsl/var/DictIterator.h"

namespace bsl{
namespace var{

    /**
    * @brief 为IVar支持无类型二进制数据而定义的结构体
    *  
    * 相当于头指针与长度的简单集合，析构时不会回收data所指内存。
    */
    struct raw_t{
        const void  *data;
        size_t      length;

        raw_t()
            :data(NULL), length(0) {}
        raw_t( const void *data_, size_t len)
            :data(data_), length(len) {}
    };

    //forward declaration
    class IVar{
    public:
        /**
        * @brief 字符串类型
        *  
        *  
        */
        typedef bsl::string         string_type;

        /**
        * @brief 字段名类型
        *  
        *  
        */
        typedef bsl::string         field_type;

        /**
        * @brief 无类型二进制数据类型
        *  
        *  
        */
        typedef raw_t               raw_type;

        /**
        * @brief 数组迭代器类型
        *  
        *  
        */
        typedef ArrayIterator       array_iterator;

        /**
        * @brief 只读数组迭代器类型
        *  
        *  
        */
        typedef ArrayConstIterator  array_const_iterator;

        /**
        * @brief 字典迭代器类型
        *  
        *  
        */
        typedef DictIterator        dict_iterator;

        /**
        * @brief 只读字典迭代器类型
        *  
        *  
        */
        typedef DictConstIterator   dict_const_iterator;

        //methods for all
        /**
         * @brief 析构函数
         *
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:49:09
        **/
        virtual ~IVar(){ } 

        /**
         * @brief 赋值运算符
         *
         * 所有IVar实现类都必须支持该方法。
         *
         * @return  IVar&  operator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:49:21
        **/
        virtual IVar& operator = ( IVar& ) = 0;

        /**
         * @brief 打印重要的内部状态，及其子IVar对象的状态
         *
         * 所有IVar实现类都必须支持该方法。
         * 该方法仅用于调试与跟踪，其内容应该容易被肉眼识别。其格式可能经常变化，不应对其内容进行监控。
         * 
         * 可选的verbose_level参数表示递归深度。0表示不递归子IVar对象，实现类应保证该函数算法复杂度为O(1)；1表示递归所有直接子IVar对象，实现类应保证该函数算法复杂度为O(size())，余类推。
         *
         * @return  string_type 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:51:26
        **/
        virtual string_type dump(size_t verbose_level=0) const = 0;

        /**
         * @brief 清空函数
         *
         * 所有IVar实现类都必须支持该方法。
         *
         * @return  void 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:51:06
        **/
        virtual void clear() = 0;

        /**
         * @brief 转化为字符串。
         *
         * 所有IVar实现类都必须支持该方法。
         * 自BSL 1.0.5后，该函数只用于转化为字符串，调试/跟踪应使用dump()
         *
         * @return  string_type 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:51:26
        **/
        virtual string_type to_string() const = 0;

        /**
         * @brief 返回实现类型的字符串表示
         *
         * 所有IVar实现类都必须支持该方法。
         *
         * @return  string_type 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:54:18
        **/
        virtual string_type get_type() const = 0; 

        //methods for all, test methods
        /**
         * @brief 返回是否为空类型
         *
         * 所有IVar实现类都必须支持该方法。
         *
         * @return  bool 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:56:50
        **/
        virtual bool is_null() const {
            return false;
        }

        /**
         * @brief 返回是否为引用类型
         *
         * 所有IVar实现类都必须支持该方法。
         *
         * @return  bool 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:57:06
        **/
        virtual bool is_ref() const {
            return false;
        }

        /**
         * @brief 返回是否为布尔类型
         * 所有IVar实现类都必须支持该方法。
         *
         * @return  bool 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/05/07 11:10:51
        **/
        virtual bool is_bool() const {
            return false;
        }
        /**
         * @brief 返回是否为数值类型
         *
         * 所有IVar实现类都必须支持该方法。
         *
         * @return  bool 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:57:14
        **/
        virtual bool is_number() const {
            return false;
        }

        /**
         * @brief 返回是否为32位整数类型
         *
         * 所有IVar实现类都必须支持该方法。
         *
         * @return  bool 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:57:24
        **/
        virtual bool is_int32() const {
            return false;
        }

        /**
         * @brief 返回是否为64位整数类型
         *
         * 所有IVar实现类都必须支持该方法。
         *
         * @return  bool 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:57:36
        **/
        virtual bool is_int64() const {
            return false;
        }

        /**
         * @brief 返回是否为双精度浮点数类型
         *
         * 所有IVar实现类都必须支持该方法。
         *
         * @return  bool 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:57:47
        **/
        virtual bool is_double() const {
            return false;
        }

        /**
         * @brief 返回是否为字符串类型
         *
         * 所有IVar实现类都必须支持该方法。
         *
         * @return  bool 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:58:11
        **/
        virtual bool is_string() const {
            return false;
        }

        /**
         * @brief 返回是否为数组类型
         *
         * 所有IVar实现类都必须支持该方法。
         *
         * @return  bool 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:58:21
        **/
        virtual bool is_array() const {
            return false;
        }

        /**
         * @brief 返回是否为字典类型
         *
         * 所有IVar实现类都必须支持该方法。
         *
         * @return  bool 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:58:28
        **/
        virtual bool is_dict() const {
            return false;
        }

        /**
         * @brief 返回是否为可调用类型
         *
         * 所有IVar实现类都必须支持该方法。
         *
         * @return  bool 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:58:37
        **/
        virtual bool is_callable() const {
            return false;
        }

        /**
         * @brief 返回是否为无类型二进制数据类型
         *
         * 所有IVar实现类都必须支持该方法
         *
         * @return  bool 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/05/03 14:26:52
        **/
        virtual bool is_raw() const {
            return false;
        }

        /*
         * 如果有需求
         virtual IVar& clone(bsl::ResourcePool) const = 0;
         */

        /**
         * @brief 所有is_bool()返回true的IVar实现类都必须支持该方法
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  bool 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/05/07 11:10:19
        **/
        virtual bool to_bool() const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }
        //methods for value
        /**
         * @brief 转化为32位整数
         *
         * 所有is_number()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若转化发生数值溢出，抛出bsl::UnderflowException / bsl::OverflowException
         *
         * @return  int 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:58:52
        **/
        virtual int to_int32() const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         * @brief 转化为64位整数
         *
         * 所有is_number()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若转化发生数值溢出，抛出bsl::UnderflowException / bsl::OverflowException
         *
         * @return  long long 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:02:42
        **/
        virtual long long to_int64() const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         * @brief 转化为双精度浮点数
         *
         * 所有is_number()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  double 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:03:06
        **/
        virtual double to_double() const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         * @brief 转化为无类型二进制数据类型
         *
         * 所有is_raw()返回true的实现类都必须支持该方法
         *
         * @return  raw_type 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/05/03 14:28:31
        **/
        virtual raw_type to_raw() const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         * @brief 所有is_bool()返回true的IVar实现类都必须支持该方法
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  IVar&  operator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/05/07 11:09:34
        **/
        virtual IVar& operator = ( bool value_ ){
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         * @brief 用32位整数赋值
         *
         * 所有is_number()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若转化发生数值溢出，抛出bsl::UnderflowException / bsl::OverflowException
         *
         * @return  IVar&  operator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:03:28
        **/
        virtual IVar& operator = ( int value_ ){
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         * @brief 用64位整数赋值
         *
         * 所有is_number()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若转化发生数值溢出，抛出bsl::UnderflowException / bsl::OverflowException
         *
         * @param [in] long   : long
         * @return  IVar&  operator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:05:23
        **/
        virtual IVar& operator = ( long long value_ ){
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         * @brief 用双精度浮点数赋值
         *
         * 所有is_number()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若转化发生数值溢出，抛出bsl::UnderflowException / bsl::OverflowException
         *
         * @return  IVar&  operator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:05:50
        **/
        virtual IVar& operator = ( double value_ ){
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         * @brief 用C风格字符串赋值
         *
         * 所有is_string()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若转化发生数值溢出，抛出bsl::UnderflowException / bsl::OverflowException
         *
         * @return  IVar&  operator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:06:16
        **/
        virtual IVar& operator = ( const char * value_ ){
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         * @brief 用字符串赋值
         *
         * 所有is_string()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若转化发生数值溢出，抛出bsl::UnderflowException / bsl::OverflowException
         *
         * @return  IVar&  operator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:06:16
        **/
        virtual IVar& operator = ( const string_type& value_ ){
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from["<<value_<<"]";
        }

        /**
         * @brief 返回C风格字符串表示
         *
         * 所有is_string()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  const char* 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/04/22 18:41:19
        **/
        virtual const char * c_str() const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         * @brief 返回C风格字符串表示的长度，不包括末尾的'\0'
         *
         * 所有is_string()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  size_t 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/04/22 18:41:54
        **/
        virtual size_t c_str_len() const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         * @brief 用无类型二进制数据类型赋值
         *
         * 所有is_raw()返回true的实现类都必须实现该方法
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  IVar&  operator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/05/03 14:32:19
        **/
        virtual IVar& operator = ( const raw_type& value_ ){
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] assign from raw[data:"<<value_.data<<", length:"<<value_.length<<"]";
        }

        //methods for array and dict
        /**
         * @brief 返回集合的元素个数。
         *
         * is_array()返回true或者is_dict()返回true的实现类必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  size_t 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:49:52
        **/
        virtual size_t size() const{
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        //methods for array
        /**
         * @brief 获取下标index处的IVar对象的引用对象
         *
         * 所有is_array()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若下标越界，返回bsl::var::Null::null;
         * 一般用于试探性获取
         *
         * @return  IVar& 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:07:04
        **/
        virtual IVar& get( size_t idx ) {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
        }

        /**
         * @brief 获取下标index处的IVar对象的引用对象
         *
         * 所有is_array()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若下标越界，返回默认值
         * 一般用于试探性获取，并且有特殊默认值要求
         *
         * @return  IVar& 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:07:04
        **/
        virtual IVar& get( size_t idx, IVar& default_value ) {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"] default["<<default_value.dump(0)<<"]";
        }

        /**
         * @brief 获取下标index处的IVar对象的引用对象
         *
         * 所有is_array()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若index >= size()，返回bsl::var::Null::null;
         * 一般用于试探性获取
         *
         * @return  const IVar& 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:12:42
        **/
        virtual const IVar& get( size_t idx ) const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
        }

        /**
         * @brief 获取下标index处的IVar对象的引用对象
         *
         * 所有is_array()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若下标越界，返回默认值
         * 一般用于试探性获取，并且有特殊默认值要求
         *
         * @return  IVar& 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:07:04
        **/
        virtual const IVar& get( size_t idx, const IVar& default_value ) const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"] default["<<default_value.dump(0)<<"]";
        }

        /**
         * @brief 设置下标绑定
         *
         * 所有is_array()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若index >= size()，则数组会自动增长到size() + 1
         *
         * @return  void
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:07:04
        **/
        virtual void set( size_t idx, IVar& value_ ){
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"] value["<<value_.dump(0)<<"]";
        }

        /**
         * @brief 删除下标绑定
         *
         * 所有is_array()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若绑定不存在，返回false；否则，返回true。
         *
         * @return  bool 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:14:29
        **/
        virtual bool del( size_t idx ){
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
        }

        /**
         * @brief 返回只读起始数组迭代器
         *
         * 所有is_array()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  array_const_iterator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:14:51
        **/
        virtual array_const_iterator array_begin() const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         * @brief 返回起始数组迭代器
         *
         * 所有is_array()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  array_iterator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:15:06
        **/
        virtual array_iterator array_begin() {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         * @brief 返回只读末尾数组迭代器
         *
         * 所有is_array()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  array_const_iterator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:15:25
        **/
        virtual array_const_iterator array_end() const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         * @brief 返回数组末尾迭代器
         *
         * 所有is_array()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  array_iterator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:15:54
        **/
        virtual array_iterator array_end() {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         * @brief 返回/设置下标绑定
         *
         * 所有is_array()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若index >= size()，抛出bsl::OutOfBoundException异常
         * 一般用于"确定性获取"（获取不到直接抛异常）
         * 试验性支持：若index < 0，等价于size() + index
         *
         * @param [in] index   : int
         * @return  const IVar& [] 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:16:21
        **/
        virtual const IVar& operator []( int idx ) const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
        }

        /**
         * @brief 返回/设置下标绑定
         *
         * 所有is_array()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若index >= size()，数组会自动增长到index + 1
         * 一般用于设置下标绑定或者确定性获取（获取不到会导致数组自动增长）
         * 试验性支持：若index < 0，等价于size() + index
         *
         * @param [in] index   : int
         * @return  IVar& [] 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:23:03
        **/
        virtual IVar& operator []( int idx ){
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
        }

        //methods for dict
        /**
         * @brief 返回字段名绑定
         *
         * 所有is_dict()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 对于不存在的字段名绑定，返回bsl::var::Null::null。
         * 一般用于试探性获取。
         *
         * @return  IVar& 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:23:15
        **/
        virtual IVar& get( const field_type& name ) {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"]";
        }

        /**
         * @brief 返回字段名绑定，若字段名不存在，返回默认值
         *
         * 所有is_dict()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 对于不存在的字段名绑定，返回指定的默认值。
         *
         * @return  IVar& 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:23:15
        **/
        virtual IVar& get( const field_type& name, IVar& default_value ) {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"] default["<<default_value.dump(0)<<"]";
        }

        /**
         * @brief 返回字段名绑定
         *
         * 所有is_dict()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若不存在该字段名绑定，返回bsl::var::Null::null;
         * 一般用于试探性获取
         *
         * @return  const IVar& 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:24:41
        **/
        virtual const IVar& get( const field_type& name ) const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"]";
        }

        /**
         * @brief 返回字段名绑定，若字段名不存在，返回默认值
         *
         * 所有is_dict()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若不存在该字段名绑定，返回指定的默认值
         * 一般用于试探性获取
         *
         * @return  IVar& 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:23:15
        **/
        virtual const IVar& get( const field_type& name, const IVar& default_value ) const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"] default["<<default_value.dump(0)<<"]";
        }

        /**
         * @brief 设置字段名绑定
         *
         * 所有is_dict()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  IVar& 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:23:15
        **/
        virtual void set( const field_type& name, IVar& value_ ){
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"] value["<<value_.dump(0)<<"]";
        }

        /**
         * @brief 删除字段名绑定
         *
         * 所有is_dict()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若绑定不存在，返回false；否则，返回true。
         *
         * @return  bool 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:26:44
        **/
        virtual bool del( const field_type& name ) {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"]";
        }

        /**
         * @brief 获取/设置字段名绑定
         *
         * 所有is_dict()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若绑定不存在，抛出bsl::KeyNotFoundException异常
         * 一般用于"确定性获取"（获取不到直接抛异常）
         *
         * @param [in] name   : const field_type&
         * @return  const IVar& [] 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:26:59
        **/
        virtual const IVar& operator []( const field_type& name ) const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"]";
        }

        /**
         * @brief 获取/设置字段名绑定
         *
         * 所有is_dict()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * 若绑定不存在，新建绑定。
         * 一般用于绑定或者确定性获取（获取不到会导致新建绑定）
         *
         * @param [in] name   : const field_type&
         * @return  IVar& [] 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:27:21
        **/
        virtual IVar& operator []( const field_type& name ){
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] name["<<name<<"]";
        }

        /**
         * @brief 返回只读起始字典迭代器
         *
         * 所有is_dict()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  dict_const_iterator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:27:52
        **/
        virtual dict_const_iterator dict_begin() const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         * @brief 返回起始字典迭代器
         *
         * 所有is_dict()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  dict_iterator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:29:20
        **/
        virtual dict_iterator dict_begin() {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         * @brief 返回只读末尾字典迭代器
         *
         * 所有is_dict()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  dict_const_iterator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:31:51
        **/
        virtual dict_const_iterator dict_end() const {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         * @brief 返回末尾字典迭代器
         *
         * 所有is_dict()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         *
         * @return  dict_iterator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:32:30
        **/
        virtual dict_iterator dict_end() {
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"]";
        }

        /**
         * @brief 调用IVar对象 
         *
         * 所有is_callable()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * params应该是一个is_array()返回true的IVar对象
         * 注：这是一个试验性方法，bsl::ResourcePool有可能被bsl::var::IFactory代替
         *
         * @return  IVar& 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:32:50
        **/
        virtual IVar& operator()(IVar& params, bsl::ResourcePool& /*rp*/ ){
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] params["<<params.dump(1)<<"]";
        }

        /**
         * @brief 调用IVar对象
         *
         * 所有is_callable()返回true的IVar实现类都必须支持该方法。
         * 若实现类不支持该操作，抛出bsl::InvalidOperationException异常
         * self可以是任意IVar对象
         * params应该是一个is_array()返回true的IVar对象
         * 注：这是一个试验性方法，bsl::ResourcePool有可能被bsl::var::IFactory代替
         *
         * @return  IVar& 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 19:33:16
        **/
        virtual IVar& operator()(IVar& self, IVar& params, bsl::ResourcePool& /*rp*/ ){
            throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] self["<<self.dump(0)<<"] params["<<params.dump(1)<<"]";
        }

    };

}}   //  namespace bsl::var

#endif  //__BSL_VAR_IVAR_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
