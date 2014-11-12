/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: Dict.h,v 1.4 2009/06/26 07:24:14 zhangfang Exp $ 
 * 
 **************************************************************************/



/**
 * @file Dict.h
 * @author chenxm(chenxiaoming@baidu.com)
 * @date 2008/09/24 01:36:18
 * @version $Revision: 1.4 $ 
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_DICT_H__
#define  __BSL_VAR_DICT_H__

#include <map>
#if __GNUC__ >= 3
#include <ext/hash_map>
#else
#include <hash_map>
#endif
#include "bsl/pool.h"
#include "bsl/var/IVar.h"
#include "bsl/var/Ref.h"
#include "bsl/var/Null.h"
#include "bsl/var/utils.h"

#if __GNUC__ < 3
namespace __gnu_cxx{
    /**
    * @brief ʹg++2.96ҲͳһΪ__gnu_cxx�����ռ�
    *  
    *  
    */
    using std::hash_map;
    using std::hash;
}
#endif

namespace bsl{
namespace var{
    //forward declarations & typedefs
    template<typename implement_t>
        class BasicDict;

    template<template<class>class Alloc>
        class __StdMapAdapter;

    template<template<class>class Alloc>
        class __GnuHashMapAdapter;

    /**
    * @brief ��װ��std::map����ʹ��bsl::pool_allocator���ֵ�
    *  
    */
    typedef BasicDict<__StdMapAdapter<bsl::pool_allocator> >    StdMapDict;

    /**
    * @brief ��װ��__gnu_cxx::hash_map����ʹ��bsl::pool_allocator���ֵ�
    *  
    *  
    */
    typedef BasicDict<__GnuHashMapAdapter<bsl::pool_allocator> >GnuHashDict;

    /**
    * @brief ���Ƽ�ʹ�õ��ֵ�
    *  
    *  Ŀǰ���Ƽ�ʹ�õ��Ƿ�װ��std::map����ʹ��bsl::pool_allocator��StdMapDict
    */
    typedef StdMapDict                                          Dict;


    template<typename implement_t>
        class BasicDict: public IVar{
        public:
            typedef IVar::string_type                       string_type;
            typedef IVar::field_type                        field_type;
            typedef typename implement_t::allocator_type    allocator_type;
            typedef DictIterator                            dict_iterator;
            typedef DictConstIterator                       dict_const_iterator;
            typedef Ref                                     reference_type;

        public:
            BasicDict()
                :_dict(), _alloc() {}

            explicit BasicDict( size_t init_capacity, const allocator_type& alloc_ = allocator_type() )
                :_dict(init_capacity, alloc_), _alloc(alloc_) {}

            explicit BasicDict( const allocator_type& alloc_ )
                :_dict(alloc_), _alloc(alloc_) {}

            BasicDict( const BasicDict& other )
                :IVar(other), _dict(other._dict), _alloc(other._alloc) {}

            BasicDict& operator = ( const BasicDict& other ){
                try{
                    _dict = other._dict;
                    _alloc= other._alloc;
                }catch(bsl::Exception& e){
                    e<<"{"<<__PRETTY_FUNCTION__<<"("<<other.dump(0)<<")}";
                    throw;
                }catch(std::exception& e){
                    throw StdException(e)<<BSL_EARG<<"{"<<__PRETTY_FUNCTION__<<"("<<other.dump(0)<<")}";
                }catch(...){
                    throw UnknownException()<<BSL_EARG<<"{"<<__PRETTY_FUNCTION__<<"("<<other.dump(0)<<")}";
                }
                return *this;
            }

            virtual ~BasicDict() {}

            virtual size_t size() const {
                return _dict.size();
            }

            virtual void clear(){
                return _dict.clear();
            }

            /**
             * @brief �ݹ��ӡ�����¼������Ϣ
             *
             * @param [in] verbose_level   : size_t
             * @return  string_type 
             * @retval   
             * @see 
             * @author chenxm
             * @date 2009/05/14 17:34:58
            **/
            virtual string_type dump(size_t verbose_level = 0) const {
                string_type res;
                if ( verbose_level == 0 ){
                    res.appendf("[bsl::var::BasicDict] this[%p] size[%zd]", this, _dict.size() ); 
                }else{
                    dump_to_string(*this, res, verbose_level, "", 0 );
                }
                return res;
            }

            virtual string_type to_string() const {
                return dump(0);
            }

            virtual BasicDict& operator = ( IVar& var ) {
                if ( typeid(var) == typeid(*this) ){
                    _dict = dynamic_cast<const BasicDict&>(var)._dict;
                }else if ( var.is_dict() ){
                    _dict.clear();
                    IVar::dict_iterator iter_ = var.dict_begin();
                    IVar::dict_iterator end  = var.dict_end();
                    for( ; iter_ != end ; ++ iter_ ){
                        _dict[iter_->key()] = iter_->value();
                    }
                }else{
                    throw bsl::InvalidOperationException()<<BSL_EARG<<"cast from "<<var.dump();
                }

                return *this;
                    
            }


            virtual string_type get_type() const {
                return "bsl::var::BasicDict";
            }

            //testers
            virtual bool is_dict() const {
                return true;
            }

            //converters
            using IVar::operator =;

            //methods for dict
            virtual IVar& get( const field_type& name ) {
                iter_impl_t iter_ = _dict.find( name );
                if ( iter_ == _dict.end() ){
                    return bsl::var::Null::null;
                }
                return iter_->second;
            }

            virtual IVar& get( const field_type& name, IVar& default_value ) {
                iter_impl_t iter_ = _dict.find( name );
                if ( iter_ == _dict.end() ){
                    return default_value;
                }
                return iter_->second;
            }

            virtual const IVar& get( const field_type& name ) const {
                const_iter_impl_t iter_ = _dict.find( name );
                if ( iter_ == _dict.end() ){
                    return bsl::var::Null::null;
                }
                return iter_->second;
            }

            virtual const IVar& get( const field_type& name, const IVar& default_value ) const {
                const_iter_impl_t iter_ = _dict.find( name );
                if ( iter_ == _dict.end() ){
                    return default_value;
                }
                return iter_->second;
            }

            virtual void set( const field_type& name, IVar& value ){
                _dict[name] = value;
            }

            virtual bool del( const field_type& name ) {
                iter_impl_t iter_ = _dict.find( name );
                if ( iter_ == _dict.end() ){
                    return false;
                }
                _dict.erase( iter_ );
                return true;
            }

            virtual dict_const_iterator dict_begin() const {
                return dict_const_iterator( _s_create_const_iterator( _dict.begin(), &_alloc ), _s_clone_const_iterator, _s_destroy_const_iterator, &_alloc );
            }

            virtual dict_iterator dict_begin() {
                return dict_iterator( _s_create_iterator( _dict.begin(), &_alloc ), _s_clone_iterator, _s_destroy_iterator, &_alloc );
            }

            virtual dict_const_iterator dict_end() const {
                return dict_const_iterator( _s_create_const_iterator( _dict.end(), &_alloc ), _s_clone_const_iterator, _s_destroy_const_iterator, &_alloc );
            }

            virtual dict_iterator dict_end() {
                return dict_iterator( _s_create_iterator( _dict.end(), &_alloc ), _s_clone_iterator, _s_destroy_iterator, &_alloc );
            }

            virtual const IVar& operator []( const field_type& name ) const {
                const_iter_impl_t iter_ = _dict.find(name);
                if ( iter_ == _dict.end() ){
                    throw bsl::KeyNotFoundException()<<BSL_EARG<<"key["<<name<<"]";
                }
                return iter_->second;
            }

            virtual IVar& operator []( const field_type& name ){
                return _dict[name];
            }

            //methods for array
#if __GNUC__ > 2
            using IVar::operator [];
            using IVar::get;
            using IVar::set;
            using IVar::del;
#else
            //avoid using bug of g++ 2.96
            virtual IVar& get( size_t idx ) {
                throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
            }

            virtual IVar& get( size_t idx, IVar& default_value ) {
                throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"] default["<<default_value.dump(0)<<"]";
            }

            virtual const IVar& get( size_t idx ) const {
                throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
            }

            virtual const IVar& get( size_t idx, const IVar& default_value ) const {
                throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"] default["<<default_value.dump(0)<<"]";
            }

            virtual void set( size_t idx, IVar& value_ ){
                throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"] value["<<value_.dump(0)<<"]";
            }

            virtual bool del( size_t idx ){
                throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
            }

            virtual const IVar& operator []( int idx ) const {
                throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
            }

            virtual IVar& operator []( int idx ){
                throw bsl::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
            }

#endif

        private:
            typedef typename implement_t::iterator          iter_impl_t;
            typedef typename implement_t::const_iterator    const_iter_impl_t;
            class DictIteratorImpl;
            class DictConstIteratorImpl;

            class DictIteratorImpl: public IDictIteratorImpl{
                friend class DictConstIteratorImpl;
            public:

                DictIteratorImpl( const iter_impl_t& iter_ )
                    :_iter(iter_){}

                virtual ~DictIteratorImpl() {
                    //pass
                }

                virtual const string_type& key() const {
                    return _iter->first;
                }

                virtual IVar& value() const {
                    return _iter->second;
                }

                virtual void iterate(){
                    ++ _iter;
                }

                virtual void assign( const IDictIteratorImpl& other ) {
                    const DictIteratorImpl *p = dynamic_cast<const DictIteratorImpl *>(&other);
                    if ( !p ){
                        throw bsl::BadCastException()<<BSL_EARG<<"from:"<<typeid(other).name()<<" to:DictIteratorImpl";
                    }
                    _iter   = p->_iter;
                }

                virtual bool equal_to( const IDictIteratorImpl& other ) const;

                virtual bool equal_to( const IDictConstIteratorImpl& other ) const;

            private:
                iter_impl_t _iter;
            };


            class DictConstIteratorImpl: public IDictConstIteratorImpl{
                friend class DictIteratorImpl;
            public:

                DictConstIteratorImpl( const const_iter_impl_t& iter_ )
                    :_iter(iter_){}

                virtual ~DictConstIteratorImpl() {
                    //pass
                }

                virtual const string_type& key() const {
                    return _iter->first;
                }

                virtual const IVar& value() const {
                    return _iter->second;
                }

                virtual void iterate(){
                    ++ _iter;
                }

                virtual void assign( const IDictIteratorImpl& other ) {
                    const DictIteratorImpl *p = dynamic_cast<const DictIteratorImpl *>(&other);
                    if ( !p ){
                        throw bsl::BadCastException()<<BSL_EARG<<"from:"<<typeid(other).name()<<" to:DictIteratorImpl";
                    }
                    _iter   = p->_iter;
                }

                virtual void assign( const IDictConstIteratorImpl& other ) {
                    const DictConstIteratorImpl *p = dynamic_cast<const DictConstIteratorImpl *>(&other);
                    if ( !p ){
                        throw bsl::BadCastException()<<BSL_EARG<<"from:"<<typeid(other).name()<<" to:DictIteratorImpl";
                    }
                    _iter   = p->_iter;
                }

                virtual bool equal_to( const IDictConstIteratorImpl& other ) const;

                virtual bool equal_to( const IDictIteratorImpl& other ) const;

            private:
                const_iter_impl_t _iter;
            };

            static IDictIteratorImpl * _s_create_iterator( const iter_impl_t& iter_, const void *p_alloc ){
                typedef typename allocator_type::template rebind<DictIteratorImpl>::other impl_alloc_t;
                IDictIteratorImpl *p = impl_alloc_t(*static_cast<const allocator_type*>(p_alloc)).allocate(1);   //throw
                new(p) DictIteratorImpl( iter_ );  //nothrow
                return p;
            }

            static IDictConstIteratorImpl * _s_create_const_iterator( const const_iter_impl_t& iter_, const void *p_alloc ){
                typedef typename allocator_type::template rebind<DictConstIteratorImpl>::other impl_alloc_t;
                IDictConstIteratorImpl *p = impl_alloc_t(*static_cast<const allocator_type*>(p_alloc)).allocate(1);   //throw
                new(p) DictConstIteratorImpl( iter_ );  //nothrow
                return p;
            }

            static IDictIteratorImpl * _s_clone_iterator( const IDictIteratorImpl *p_other, const void *p_alloc ){
                typedef typename allocator_type::template rebind<DictIteratorImpl>::other impl_alloc_t;
                const DictIteratorImpl *psrc = dynamic_cast<const DictIteratorImpl*>(p_other);
                if ( !psrc ){
                    throw bsl::BadCastException()<<BSL_EARG<<"from:"<<typeid(*p_other).name()<<" to:DictIteratorImpl";
                }
                IDictIteratorImpl *p = impl_alloc_t(*static_cast<const allocator_type*>(p_alloc)).allocate(1);   //throw
                new(p) DictIteratorImpl(*psrc);  
                return p;
            }

            static IDictConstIteratorImpl * _s_clone_const_iterator( const IDictConstIteratorImpl *p_other, const void *p_alloc ){
                typedef typename allocator_type::template rebind<DictConstIteratorImpl>::other impl_alloc_t;
                const DictConstIteratorImpl *psrc = dynamic_cast<const DictConstIteratorImpl*>(p_other);
                if ( !psrc ){
                    throw bsl::BadCastException()<<BSL_EARG<<"from:"<<typeid(*p_other).name()<<" to:DictIteratorImpl";
                }
                IDictConstIteratorImpl *p = impl_alloc_t(*static_cast<const allocator_type*>(p_alloc)).allocate(1);   //throw
                new(p) DictConstIteratorImpl(*psrc);  
                return p;
            }

            static void _s_destroy_iterator( IDictIteratorImpl * p, const void *p_alloc){
                typedef typename allocator_type::template rebind<DictIteratorImpl>::other impl_alloc_t;
                DictIteratorImpl *_p = dynamic_cast<DictIteratorImpl *>(p); 
                if ( _p ){
                    _p->~DictIteratorImpl();
                    impl_alloc_t(*static_cast<const allocator_type*>(p_alloc)).deallocate( _p, 1 );
                }
            }

            static void _s_destroy_const_iterator( IDictConstIteratorImpl * p, const void *p_alloc ){
                typedef typename allocator_type::template rebind<DictConstIteratorImpl>::other impl_alloc_t;
                DictConstIteratorImpl *_p = dynamic_cast<DictConstIteratorImpl *>(p); 
                if ( _p ){
                    _p->~DictConstIteratorImpl();
                    impl_alloc_t(*static_cast<const allocator_type*>(p_alloc)).deallocate( _p, 1 );
                }
            }

            implement_t     _dict;
            allocator_type  _alloc;   //TODO��Ϊ�˼�ֱ�ۣ�û����EBO�Ż����Ժ���Լ���
        };

    template<typename implement_t>
        inline bool BasicDict<implement_t>::DictIteratorImpl::equal_to( const IDictIteratorImpl& other ) const {
            const DictIteratorImpl *p = dynamic_cast<const DictIteratorImpl *>(&other);
            return p != NULL  && _iter == p->_iter;
        }

    template<typename implement_t>
        inline bool BasicDict<implement_t>::DictIteratorImpl::equal_to( const IDictConstIteratorImpl& other ) const {
            const DictConstIteratorImpl *p = dynamic_cast<const DictConstIteratorImpl *>(&other);
            return p != NULL  && p->_iter == _iter;
        }

    template<typename implement_t>
        inline bool BasicDict<implement_t>::DictConstIteratorImpl::equal_to( const IDictIteratorImpl& other ) const {
            const DictIteratorImpl *p = dynamic_cast<const DictIteratorImpl *>(&other);
            return p != NULL  && _iter == p->_iter; 
        }

    template<typename implement_t>
        inline bool BasicDict<implement_t>::DictConstIteratorImpl::equal_to( const IDictConstIteratorImpl& other ) const {
            const DictConstIteratorImpl *p = dynamic_cast<const DictConstIteratorImpl *>(&other);
            return p != NULL  && _iter == p->_iter;
        }

    /**
    * @brief ����һ��std::map�Ľӿڣ��ṩһ������allocator�Ĺ��캯��
    *  
    *  ���͵�adapterģʽ
    *  ��adapter����ʹ������ʱ����û����ô����:-P
    *  
    */
    template<template<class>class Alloc>
        class __StdMapAdapter: public std::map<IVar::string_type, bsl::var::Ref, std::less<IVar::string_type>, Alloc<std::pair<IVar::string_type, bsl::var::Ref> > >{
        public:
            typedef std::map<IVar::string_type, bsl::var::Ref, std::less<IVar::string_type>, Alloc<std::pair<IVar::string_type, bsl::var::Ref> > > base_type;
            typedef typename base_type::allocator_type  allocator_type;
            typedef typename base_type::iterator        iterator;
            typedef typename base_type::const_iterator  const_iterator;

            /**
             * @brief Ĭ�Ϲ��캯��
             *
             * @see 
             * @author chenxm
             * @date 2009/04/08 11:22:53
            **/
            __StdMapAdapter()
                :base_type(){}

            /**
             * @brief ʹ��allocator��ʼ���Ĺ��캯��
             *
             * @param [in] alloc_   : const allocator_type&
             * @return  explicit 
             * @retval   
             * @see 
             * @author chenxm
             * @date 2009/04/08 11:23:05
            **/
            explicit __StdMapAdapter( const allocator_type& alloc_ )
                :base_type(std::less<IVar::string_type>(), alloc_ ){}

            /**
             * @brief ʹ��init_capacity��allocator��ʼ���Ĺ��캯��
             *
             * ����std::map��init_capacityû�����壬ֱ�Ӻ���
             * @param [in] init_capacity   : size_t
             * @param [in] alloc_   : const allocator_type&
             * @return  explicit 
             * @retval   
             * @see 
             * @author chenxm
             * @date 2009/04/08 12:02:53
            **/
            explicit __StdMapAdapter( size_t /*init_capacity*/, const allocator_type& alloc_ )
                :base_type(std::less<IVar::string_type>(), alloc_ ){}

            //inherit everything else
        };

    /**
    * @brief ����һ��__gnu_cxx::hash_map�Ľӿڣ��ṩһ������allocator�Ĺ��캯��
    *  
    *  ���͵�adapterģʽ
    *  ��adapter����ʹ������ʱ����û����ô����:-P
    *  
    */
    template<template<class>class Alloc>
        class __GnuHashMapAdapter: public __gnu_cxx::hash_map<IVar::string_type, bsl::var::Ref, __gnu_cxx::hash<IVar::string_type>, std::equal_to<IVar::string_type>, Alloc<bsl::var::Ref> > {
        public:
            typedef __gnu_cxx::hash_map<IVar::string_type, bsl::var::Ref, __gnu_cxx::hash<IVar::string_type>, std::equal_to<IVar::string_type>, Alloc<bsl::var::Ref> > base_type;
            typedef typename base_type::allocator_type  allocator_type;
            typedef typename base_type::iterator        iterator;
            typedef typename base_type::const_iterator  const_iterator;

            /**
             * @brief Ĭ�Ϲ��캯��
             *
             * @see 
             * @author chenxm
             * @date 2009/04/08 11:22:15
            **/
            __GnuHashMapAdapter()
                :base_type(){}

            /**
             * @brief ʹ��allocator��ʼ���Ĺ��캯��
             *
             * 100��__gnu_cxx::hash_mapʹ�õ�Ĭ������ֵ
             * @param [in] alloc_   : const allocator_type&
             * @return  explicit 
             * @retval   
             * @see 
             * @author chenxm
             * @date 2009/04/08 11:22:27
            **/
            explicit __GnuHashMapAdapter( const allocator_type& alloc_ )
                :base_type(100, typename base_type::hasher(), typename base_type::key_equal(), alloc_ ){}

            /**
             * @brief ʹ��init_capacity��allocator��ʼ���Ĺ��캯��
             *
             * @param [in] init_capacity   : size_t
             * @param [in] alloc_   : const allocator_type&
             * @return  explicit 
             * @retval   
             * @see 
             * @author chenxm
             * @date 2009/04/08 11:58:52
            **/
            explicit __GnuHashMapAdapter( size_t init_capacity, const allocator_type& alloc_ )
                :base_type(init_capacity, typename base_type::hasher(), typename base_type::key_equal(), alloc_ ){}

            //inherit everything else
        };
}}   //namespace bsl::var

#endif  //__BSL_VAR_DICT_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
