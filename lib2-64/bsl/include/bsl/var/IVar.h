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
    * @brief ΪIVar֧�������Ͷ��������ݶ�����Ľṹ��
    *  
    * �൱��ͷָ���볤�ȵļ򵥼��ϣ�����ʱ�������data��ָ�ڴ档
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
        * @brief �ַ�������
        *  
        *  
        */
        typedef bsl::string         string_type;

        /**
        * @brief �ֶ�������
        *  
        *  
        */
        typedef bsl::string         field_type;

        /**
        * @brief �����Ͷ�������������
        *  
        *  
        */
        typedef raw_t               raw_type;

        /**
        * @brief �������������
        *  
        *  
        */
        typedef ArrayIterator       array_iterator;

        /**
        * @brief ֻ���������������
        *  
        *  
        */
        typedef ArrayConstIterator  array_const_iterator;

        /**
        * @brief �ֵ����������
        *  
        *  
        */
        typedef DictIterator        dict_iterator;

        /**
        * @brief ֻ���ֵ����������
        *  
        *  
        */
        typedef DictConstIterator   dict_const_iterator;

        //methods for all
        /**
         * @brief ��������
         *
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:49:09
        **/
        virtual ~IVar(){ } 

        /**
         * @brief ��ֵ�����
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
         * @return  IVar&  operator 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:49:21
        **/
        virtual IVar& operator = ( IVar& ) = 0;

        /**
         * @brief ��ӡ��Ҫ���ڲ�״̬��������IVar�����״̬
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         * �÷��������ڵ�������٣�������Ӧ�����ױ�����ʶ�����ʽ���ܾ����仯����Ӧ�������ݽ��м�ء�
         * 
         * ��ѡ��verbose_level������ʾ�ݹ���ȡ�0��ʾ���ݹ���IVar����ʵ����Ӧ��֤�ú����㷨���Ӷ�ΪO(1)��1��ʾ�ݹ�����ֱ����IVar����ʵ����Ӧ��֤�ú����㷨���Ӷ�ΪO(size())�������ơ�
         *
         * @return  string_type 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:51:26
        **/
        virtual string_type dump(size_t verbose_level=0) const = 0;

        /**
         * @brief ��պ���
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         *
         * @return  void 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:51:06
        **/
        virtual void clear() = 0;

        /**
         * @brief ת��Ϊ�ַ�����
         *
         * ����IVarʵ���඼����֧�ָ÷�����
         * ��BSL 1.0.5�󣬸ú���ֻ����ת��Ϊ�ַ���������/����Ӧʹ��dump()
         *
         * @return  string_type 
         * @retval   
         * @see 
         * @author chenxm
         * @date 2009/02/04 18:51:26
        **/
        virtual string_type to_string() const = 0;

        /**
         * @brief ����ʵ�����͵��ַ�����ʾ
         *
         * ����IVarʵ���඼����֧�ָ÷�����
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
         * @brief �����Ƿ�Ϊ������
         *
         * ����IVarʵ���඼����֧�ָ÷�����
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
         * @brief �����Ƿ�Ϊ��������
         *
         * ����IVarʵ���඼����֧�ָ÷�����
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
         * @brief �����Ƿ�Ϊ��������
         * ����IVarʵ���඼����֧�ָ÷�����
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
         * @brief �����Ƿ�Ϊ��ֵ����
         *
         * ����IVarʵ���඼����֧�ָ÷�����
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
         * @brief �����Ƿ�Ϊ32λ��������
         *
         * ����IVarʵ���඼����֧�ָ÷�����
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
         * @brief �����Ƿ�Ϊ64λ��������
         *
         * ����IVarʵ���඼����֧�ָ÷�����
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
         * @brief �����Ƿ�Ϊ˫���ȸ���������
         *
         * ����IVarʵ���඼����֧�ָ÷�����
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
         * @brief �����Ƿ�Ϊ�ַ�������
         *
         * ����IVarʵ���඼����֧�ָ÷�����
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
         * @brief �����Ƿ�Ϊ��������
         *
         * ����IVarʵ���඼����֧�ָ÷�����
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
         * @brief �����Ƿ�Ϊ�ֵ�����
         *
         * ����IVarʵ���඼����֧�ָ÷�����
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
         * @brief �����Ƿ�Ϊ�ɵ�������
         *
         * ����IVarʵ���඼����֧�ָ÷�����
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
         * @brief �����Ƿ�Ϊ�����Ͷ�������������
         *
         * ����IVarʵ���඼����֧�ָ÷���
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
         * ���������
         virtual IVar& clone(bsl::ResourcePool) const = 0;
         */

        /**
         * @brief ����is_bool()����true��IVarʵ���඼����֧�ָ÷���
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief ת��Ϊ32λ����
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�bsl::UnderflowException / bsl::OverflowException
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
         * @brief ת��Ϊ64λ����
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�bsl::UnderflowException / bsl::OverflowException
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
         * @brief ת��Ϊ˫���ȸ�����
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief ת��Ϊ�����Ͷ�������������
         *
         * ����is_raw()����true��ʵ���඼����֧�ָ÷���
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
         * @brief ����is_bool()����true��IVarʵ���඼����֧�ָ÷���
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief ��32λ������ֵ
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�bsl::UnderflowException / bsl::OverflowException
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
         * @brief ��64λ������ֵ
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�bsl::UnderflowException / bsl::OverflowException
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
         * @brief ��˫���ȸ�������ֵ
         *
         * ����is_number()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�bsl::UnderflowException / bsl::OverflowException
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
         * @brief ��C����ַ�����ֵ
         *
         * ����is_string()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�bsl::UnderflowException / bsl::OverflowException
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
         * @brief ���ַ�����ֵ
         *
         * ����is_string()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ��ת��������ֵ������׳�bsl::UnderflowException / bsl::OverflowException
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
         * @brief ����C����ַ�����ʾ
         *
         * ����is_string()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief ����C����ַ�����ʾ�ĳ��ȣ�������ĩβ��'\0'
         *
         * ����is_string()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief �������Ͷ������������͸�ֵ
         *
         * ����is_raw()����true��ʵ���඼����ʵ�ָ÷���
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief ���ؼ��ϵ�Ԫ�ظ�����
         *
         * is_array()����true����is_dict()����true��ʵ�������֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief ��ȡ�±�index����IVar��������ö���
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ���±�Խ�磬����bsl::var::Null::null;
         * һ��������̽�Ի�ȡ
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
         * @brief ��ȡ�±�index����IVar��������ö���
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ���±�Խ�磬����Ĭ��ֵ
         * һ��������̽�Ի�ȡ������������Ĭ��ֵҪ��
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
         * @brief ��ȡ�±�index����IVar��������ö���
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ��index >= size()������bsl::var::Null::null;
         * һ��������̽�Ի�ȡ
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
         * @brief ��ȡ�±�index����IVar��������ö���
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ���±�Խ�磬����Ĭ��ֵ
         * һ��������̽�Ի�ȡ������������Ĭ��ֵҪ��
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
         * @brief �����±��
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ��index >= size()����������Զ�������size() + 1
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
         * @brief ɾ���±��
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ���󶨲����ڣ�����false�����򣬷���true��
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
         * @brief ����ֻ����ʼ���������
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief ������ʼ���������
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief ����ֻ��ĩβ���������
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief ��������ĩβ������
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief ����/�����±��
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ��index >= size()���׳�bsl::OutOfBoundException�쳣
         * һ������"ȷ���Ի�ȡ"����ȡ����ֱ�����쳣��
         * ������֧�֣���index < 0���ȼ���size() + index
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
         * @brief ����/�����±��
         *
         * ����is_array()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ��index >= size()��������Զ�������index + 1
         * һ�����������±�󶨻���ȷ���Ի�ȡ����ȡ�����ᵼ�������Զ�������
         * ������֧�֣���index < 0���ȼ���size() + index
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
         * @brief �����ֶ�����
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ���ڲ����ڵ��ֶ����󶨣�����bsl::var::Null::null��
         * һ��������̽�Ի�ȡ��
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
         * @brief �����ֶ����󶨣����ֶ��������ڣ�����Ĭ��ֵ
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ���ڲ����ڵ��ֶ����󶨣�����ָ����Ĭ��ֵ��
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
         * @brief �����ֶ�����
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * �������ڸ��ֶ����󶨣�����bsl::var::Null::null;
         * һ��������̽�Ի�ȡ
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
         * @brief �����ֶ����󶨣����ֶ��������ڣ�����Ĭ��ֵ
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * �������ڸ��ֶ����󶨣�����ָ����Ĭ��ֵ
         * һ��������̽�Ի�ȡ
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
         * @brief �����ֶ�����
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief ɾ���ֶ�����
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ���󶨲����ڣ�����false�����򣬷���true��
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
         * @brief ��ȡ/�����ֶ�����
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ���󶨲����ڣ��׳�bsl::KeyNotFoundException�쳣
         * һ������"ȷ���Ի�ȡ"����ȡ����ֱ�����쳣��
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
         * @brief ��ȡ/�����ֶ�����
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * ���󶨲����ڣ��½��󶨡�
         * һ�����ڰ󶨻���ȷ���Ի�ȡ����ȡ�����ᵼ���½��󶨣�
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
         * @brief ����ֻ����ʼ�ֵ������
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief ������ʼ�ֵ������
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief ����ֻ��ĩβ�ֵ������
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief ����ĩβ�ֵ������
         *
         * ����is_dict()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
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
         * @brief ����IVar���� 
         *
         * ����is_callable()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * paramsӦ����һ��is_array()����true��IVar����
         * ע������һ�������Է�����bsl::ResourcePool�п��ܱ�bsl::var::IFactory����
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
         * @brief ����IVar����
         *
         * ����is_callable()����true��IVarʵ���඼����֧�ָ÷�����
         * ��ʵ���಻֧�ָò������׳�bsl::InvalidOperationException�쳣
         * self����������IVar����
         * paramsӦ����һ��is_array()����true��IVar����
         * ע������һ�������Է�����bsl::ResourcePool�п��ܱ�bsl::var::IFactory����
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
