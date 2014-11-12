/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: Ref.h,v 1.4 2009/06/26 07:24:14 zhangfang Exp $ 
 * 
 **************************************************************************/



/**
 * @file Ref.h
 * @author chenxm(chenxiaoming@baidu.com)
 * @date 2008/09/24 01:30:35
 * @version $Revision: 1.4 $ 
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_REF_H__
#define  __BSL_VAR_REF_H__

#include "bsl/var/IVar.h"
#include "bsl/var/Ref.h"
#include "bsl/var/Null.h"

namespace bsl{
namespace var{
    class Ref: public IVar{
    public:
        typedef IVar::string_type   string_type;
        typedef IVar::field_type    field_type;

        //special methods
        Ref( )
            :_p(&Null::null) {}

        Ref( const Ref& other )
            :IVar(other), _p(other._p) { }

        Ref( IVar& var )
            :_p(&var){
            if ( var.is_ref() ){
                Ref * __p = dynamic_cast<Ref*>(&var);
                if ( NULL != __p ){
                    //make a shortcut
                    _p = &__p->ref();
                }
            }
        }

        /**
         * @brief Ref is a kind of "weak ref" which do nothing on destructor
         *
         * @see 
         * @author chenxm
         * @date 2008/09/25 12:08:53
        **/
        ~Ref(){
            //pass
        }

        Ref& operator = ( const Ref& other ){
            _p = other._p;
            return *this;
        }

        virtual IVar& ref() const {
            return *_p;
        }

        //methods for all
        virtual void clear() {
            return _p->clear();
        }

        virtual string_type dump(size_t verbose_level = 0) const {
            string_type res;
            res.appendf("@%p: ", _p);
            res.append(_p->dump(verbose_level));
            return res;
        }

        virtual string_type to_string() const {
            return _p->to_string();
        }

        virtual string_type get_type() const {
            string_type res("bsl::var::Ref(");
            res.append(_p->get_type());
            res.append(")");
            return res;
        }

        virtual Ref& operator = ( IVar& var ){
            if ( var.is_ref() ){
                Ref * __p = dynamic_cast<Ref*>(&var);
                if ( NULL != __p ){
                    //make a shortcut
                    _p = &__p->ref();
                }else{
                    //unknown ref type
                    _p = &var;
                }
            }else{
                //non-ref value;
                _p = &var;
            }
            return *this;
        }


        //methods for all, test methods
        virtual bool is_null() const {
            return _p->is_null();
        }

        virtual bool is_ref() const {
            return true;
        }
        
        virtual bool is_bool() const {
            return _p->is_bool();
        }

        virtual bool is_number() const {
            return _p->is_number();
        }

        virtual bool is_int32() const {
            return _p->is_int32();
        }

        virtual bool is_int64() const {
            return _p->is_int64();
        }

        virtual bool is_double() const {
            return _p->is_double();
        }

        virtual bool is_string() const {
            return _p->is_string();
        }

        virtual bool is_array() const {
            return _p->is_array();
        }

        virtual bool is_dict() const {
            return _p->is_dict();
        }

        virtual bool is_callable() const {
            return _p->is_callable();
        }

        virtual bool is_raw() const {
            return _p->is_raw();
        }

        //methods for value
        virtual bool to_bool() const {
            return _p->to_bool();  
        }

        virtual int to_int32() const {
            return _p->to_int32();  
        }

        virtual long long to_int64() const {
            return _p->to_int64();  
        }

        virtual double to_double() const {
            return _p->to_double();
        }

        virtual raw_type to_raw() const {
            return _p->to_raw();
        }

        virtual Ref& operator = ( bool val ){
            _p->operator = ( val );
            return *this;
        }

        virtual Ref& operator = ( int val ){
            _p->operator = ( val );
            return *this;
        }

        virtual Ref& operator = ( long long val ){
            _p->operator = ( val );
            return *this;
        }

        virtual Ref& operator = ( double val ){
            _p->operator = ( val );
            return *this;
        }

        virtual Ref& operator = ( const char *val ){
            _p->operator = ( val );
            return *this;
        }

        virtual Ref& operator = ( const string_type& val ){
            _p->operator = ( val );
            return *this;
        }

        virtual Ref& operator = ( const raw_type& val ){
            _p->operator = ( val );
            return *this;
        }
        
        //methods for string
        virtual const char *c_str() const {
            return _p->c_str();
        }

        virtual size_t c_str_len() const {
            return _p->c_str_len();
        }

        //methods for array or dict
        virtual size_t size() const {
            return _p->size();
        }

        //methods for array
        virtual IVar& get( size_t index ) {
            return _p->get(index);
        }

        virtual IVar& get( size_t index, IVar& default_value ) {
            return _p->get(index, default_value);
        }

        virtual const IVar& get( size_t index ) const {
            return _p->get(index);
        }

        virtual const IVar& get( size_t index, const IVar& default_value ) const {
            return _p->get(index,default_value);
        }

        virtual void set( size_t index, IVar& value ){
            return _p->set( index, value );
        }

        virtual bool del( size_t index ){
            return _p->del(index);
        }

        virtual array_const_iterator array_begin() const {
            return const_cast<const IVar *>(_p)->array_begin();
        }

        virtual array_iterator array_begin() {
            return _p->array_begin();
        }

        virtual array_const_iterator array_end() const {
            return const_cast<const IVar *>(_p)->array_end();
        }

        virtual array_iterator array_end() {
            return _p->array_end();
        }

        virtual const IVar& operator []( int index ) const {
            return _p->operator [](index);
        }

        virtual IVar& operator []( int index ){
            return _p->operator [](index); 
        }

        //methods for dict
        virtual IVar& get( const field_type& name ) {
            return _p->get(name);
        }

        virtual IVar& get( const field_type& name, IVar& default_value ) {
            return _p->get(name, default_value);
        }

        virtual const IVar& get( const field_type& name ) const {
            return _p->get(name);
        }

        virtual const IVar& get( const field_type& name, const IVar& default_value ) const {
            return _p->get(name, default_value);
        }

        virtual void set( const field_type& name, IVar& value ){
            return _p->set(name, value);
        }

        virtual bool del( const field_type& name ){
            return _p->del(name);
        }

        virtual const IVar& operator []( const field_type& name ) const {
            return _p->operator [](name);
        }

        virtual IVar& operator []( const field_type& name ){
            return _p->operator []( name );
        }

        virtual dict_const_iterator dict_begin() const {
            return const_cast<const IVar *>(_p)->dict_begin();
        }

        virtual dict_iterator dict_begin() {
            return _p->dict_begin();
        }

        virtual dict_const_iterator dict_end() const {
            return const_cast<const IVar *>(_p)->dict_end();
        }

        virtual dict_iterator dict_end() {
            return _p->dict_end();
        }

        virtual IVar& operator()(IVar& param, bsl::ResourcePool& rp ){
            return (*_p)(param, rp);
        }

        virtual IVar& operator()(IVar& self, IVar& param, bsl::ResourcePool& rp ){
            return (*_p)(self, param, rp);
        }
    private:
        IVar * _p;
    };

}}   //namespace bsl::var

#endif  //__BSL_VAR_REF_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
