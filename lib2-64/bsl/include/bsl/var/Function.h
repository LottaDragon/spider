/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: Function.h,v 1.4 2009/06/26 07:24:14 zhangfang Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file Function.h
 * @author chenxm(chenxiaoming@baidu.com)
 * @date 2008/09/24 20:53:41
 * @version $Revision: 1.4 $ 
 * @brief 
 *  
 **/
#ifndef  __BSL_VAR_FUNCTION_H__
#define  __BSL_VAR_FUNCTION_H__
#include "bsl/var/IVar.h"
#include "bsl/var/Ref.h"

namespace bsl{
namespace var{
    class Function: public IVar{
    public:
        typedef IVar::string_type          string_type;
        typedef IVar::field_type           field_type;
        typedef IVar& (* function_type )(IVar&, bsl::ResourcePool& );
        typedef IVar::array_iterator       array_iterator;
        typedef IVar::array_const_iterator array_const_iterator;

        //special methods
        Function( function_type func, const string_type& name )
            :IVar(), _func(func), _name(name){ } 

        Function( const Function& other )
            :IVar(other), _func(other._func), _name(other._name){ }

        virtual ~Function(){ } 

        Function& operator = ( const Function& other ){
            _func = other._func;
            _name = other._name;
            return *this;
        }

        //methods for all
        virtual void clear(){
            // pass
        }

        virtual string_type dump(size_t /*verbose_level*/ = 0) const {
            return string_type("[bsl::var::Function]").append(_name);
        }

        virtual string_type to_string() const {
            return _name;
        }

        virtual string_type get_type() const {
            return "bsl::var::Function";
        }

        virtual Function& operator = ( IVar& var ){
            Function * vfp = dynamic_cast<Function*>(&var);
            if ( vfp == NULL ){
                throw bsl::InvalidOperationException()<<BSL_EARG<<"cast from "<<var.dump();
            }else{
                _name = vfp->_name;
                _func = vfp->_func;
                return *this;
            }
            
        }

        virtual bool is_callable() const {
            return true;
        }

        //converters
        using IVar::operator =;

        virtual IVar& operator()(IVar& args, bsl::ResourcePool& _rp ){
            return (*_func)(args, _rp);
        }

        virtual IVar& operator()(IVar& /*self*/, IVar& args, bsl::ResourcePool& _rp ){
            return (*_func)(args, _rp);
        }

    private:
        function_type   _func;
        string_type     _name;
    };

}}   //namespace bsl::var
#endif  //__BSL_VAR_FUNCTION_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
