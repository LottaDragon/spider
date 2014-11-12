/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: mc_pack_c.h,v 1.1.2.14 2009/04/02 08:06:34 yufan Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file mc_pack_c.h
 * @author yufan(com@baidu.com)
 * @date 2008/11/25 15:53:53
 * @version $Revision: 1.1.2.14 $ 
 * @brief 
 *  
 **/


#ifndef  __MC_PACK_C_H_
#define  __MC_PACK_C_H_


#ifdef __cplusplus
extern "C" {
#endif
#include "mc_pack_def.h"

/** convert pointer to errno
 *
 * @param [in] ptr   : const void*
 * @return  inline int 
 * @retval  errno
 *		- 0 : good pointer
 *		- <0 : errno, see mc_pack_error_t
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/13 17:32:48
**/
static inline int MC_PACK_PTR_ERR(const void *ptr)
{
	if (NULL == ptr) {
		return MC_PE_NULL_POINTER;
	}

	return (((long)ptr >= -255 && (long)ptr < 0) ? (long)ptr : 0);
}

/**
 * @brief get pack size
 *		取得 pack 的大小，只适用于 pack，不适用于 array/object
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 结构的指针
 * @return  int 
 * @retval errno   
 *		- >0 : pack size
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/29 13:16:59
**/
int mc_pack_get_size(const mc_pack_t * ppack);

/**
 * @brief get item count of pack
 *		取得 pack 的 item count，可用于 array/object
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 结构的指针
 * @return  int 
 * @retval errno   
 *		- >=0 : item count
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/29 13:38:59
**/
int mc_pack_get_item_count(const mc_pack_t * ppack);

/**
 * @brief init a array item in a pack
 *		在一个 pack 中创建一个数组型的 item
 *
 * @param [in] ppack   : mc_pack_t*
 *		datapack 指针
 * @param [in] name   : const char*
 *		数组名，ppack 为数组时 name 必须为 NULL
 * @return  mc_pack_t* 
 *		指向 array 的指针
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/26 22:20:50
**/
mc_pack_t *mc_pack_put_array(mc_pack_t * ppack, const char *name);

/**
 * @brief get array from pack
 *		从 pack 中取得 array 的指针
 *
 * @param [in] ppack   : const mc_pack_t*
 *		datapack 指针
 * @param [in] name   : const char*
 *		数组名
 * @return  mc_pack_t* 
 *		指向 array 的指针
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/02 15:31:21
**/
mc_pack_t *mc_pack_get_array(const mc_pack_t * ppack, const char *name);

/**
 * @brief get array from array
 *		从 array 中取得子 array 的指针
 *
 * @param [in] ppack   : const mc_pack_t*
 *		array 指针
 * @param [in] arr_index   : u_int
 * @return  mc_pack_t* 
 *		指向 array 的指针
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/03 14:04:51
**/
mc_pack_t *mc_pack_get_array_arr(const mc_pack_t * ppack, u_int arr_index);

/**
 * @brief init a object item in a pack
 *		在一个 pack 中创建一个结构类型的 item
 *
 * @param [in] ppack   : mc_pack_t*
 *		datapack 指针
 * @param [in] name   : const char*
 *		结构名，ppack 为数组时 name 必须为 NULL
 * @return  mc_pack_t* 
 *		指向 object 的指针
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/26 22:24:28
**/
mc_pack_t *mc_pack_put_object(mc_pack_t * ppack, const char *name);

/**
 * @brief get object from pack
 *		从 pack 中取得 object 的指针
 *
 * @param [in] ppack   : const mc_pack_t*
 *		datapack 指针
 * @param [in] name   : const char*
 *		结构名
 * @return  mc_pack_t* 
 *		指向 object 的指针
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/02 15:33:17
**/
mc_pack_t *mc_pack_get_object(const mc_pack_t * ppack, const char *name);

/**
 * @brief get object from array
 *		从 array 中取得 object 的指针
 *
 * @param [in] ppack   : const mc_pack_t*
 *		array 指针
 * @param [in] arr_index   : u_int
 * @return  mc_pack_t* 
 *		指向 object 的指针
 * @retval MC_PACK_PTR_ERR(retval)
 * 		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/03 14:07:49
**/
mc_pack_t *mc_pack_get_object_arr(const mc_pack_t * ppack, u_int arr_index);

/**
 * @brief check if a pointer is valid
 *		判断一个指针指向的是否为合法的 mc_pack_t 或 array/object mc_pack_item_t
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 array/object 的指针
 * @param [in] buf_size   : u_int
 * @return  int 
 * @retval validation
 *		- 1 : valid
 *		- 0 : invalid
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/27 15:35:00
**/
int mc_pack_valid(const mc_pack_t * ppack);

/**
 * @brief 
 * 放入一个32位浮点数
 * @param [in/out] ppack   : mc_pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] value   : float
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/27 16:42:05
**/
int mc_pack_put_float(mc_pack_t * ppack, const char *name, float value);
/**
 * @brief 
 * 从对象取出一个32位浮点数
 * @param [in/out] ppack   : const mc_pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] result   : float*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/27 16:42:48
**/
int mc_pack_get_float(const mc_pack_t * ppack, const char *name, float * result);

/**
 * @brief 
 * 从数组中取出浮点数
 * @param [in/out] ppack   : const mc_pack_t*
 * @param [in/out] arr_index   : u_int
 * @param [in/out] result   : float*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/27 16:42:58
**/
int mc_pack_get_float_arr(const mc_pack_t * ppack, u_int arr_index, float * result);

/**
 * @brief 
 * 放入一个64位浮点数
 * @param [in/out] ppack   : mc_pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] value   : double
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/27 16:43:39
**/
int mc_pack_put_double(mc_pack_t * ppack, const char *name, double value);

/**
 * @brief 
 * 从对象中取出64位浮点数
 * @param [in/out] ppack   : const mc_pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] result   : double*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/27 16:43:53
**/
int mc_pack_get_double(const mc_pack_t * ppack, const char *name, double * result);

/**
 * @brief 
 * 从数组中取出浮点数
 * @param [in/out] ppack   : const mc_pack_t*
 * @param [in/out] arr_index   : u_int
 * @param [in/out] result   : double*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/27 16:44:05
**/
int mc_pack_get_double_arr(const mc_pack_t * ppack, u_int arr_index, double * result);


/**
 * @brief 
 * 放入一个bool值，1字节长。注意读取时会忽略具体数值，仅保留bool值
 * @param [in/out] ppack   : mc_pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] value   : mc_bool_t
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/27 16:44:40
**/
int mc_pack_put_bool(mc_pack_t * ppack, const char *name, mc_bool_t value);

/**
 * @brief 
 * 从对象中读取bool值
 * @param [in/out] ppack   : const mc_pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] value   : mc_bool_t*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/27 16:45:47
**/
int mc_pack_get_bool(const mc_pack_t * ppack, const char *name, mc_bool_t * value);

/**
 * @brief 
 * 从数组中读取bool值
 * @param [in/out] ppack   : const mc_pack_t*
 * @param [in/out] arr_index   : u_int
 * @param [in/out] value   : mc_bool_t*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/27 16:46:05
**/
int mc_pack_get_bool_arr(const mc_pack_t * ppack, u_int arr_index, mc_bool_t * value);

/**
 * @brief 
 * 放入一个空值
 * @param [in/out] ppack   : mc_pack_t*
 * @param [in/out] name   : const char*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/27 16:46:25
**/
int mc_pack_put_null(mc_pack_t * ppack, const char *name);

/**
 * @brief 
 * 判断一个对象的字段是否为空
 * @param [in/out] ppack   : const mc_pack_t*
 * @param [in/out] name   : const char*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/27 16:46:39
**/
int mc_pack_is_null(const mc_pack_t * ppack, const char *name);

/**
 * @brief 
 * 判断数组的一个成员是否为空
 * @param [in/out] ppack   : const mc_pack_t*
 * @param [in/out] arr_index   : u_int
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/27 16:47:00
**/
int mc_pack_is_null_arr(const mc_pack_t * ppack, u_int arr_index);

/**
 * @brief 
 * 从item中取出float值，会自动做类型转换，从double转为float时不判断范围
 * @param [in/out] pitem   : const mc_pack_item_t*
 * @param [in/out] res   : float*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/27 16:47:25
**/
int mc_pack_get_float_from_item(const mc_pack_item_t *pitem, float *res);

/**
 * @brief 
 * 从item中取出double值
 * @param [in/out] pitem   : const mc_pack_item_t*
 * @param [in/out] res   : double*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/27 16:48:09
**/
int mc_pack_get_double_from_item(const mc_pack_item_t *pitem, double *res);
/**
 * @brief 
 * 从item中取出bool值
 * @param [in/out] pitem   : const mc_pack_item_t*
 * @param [in/out] res   : mc_bool_t*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/03/04 17:35:27
**/
int mc_pack_get_bool_from_item(const mc_pack_item_t *pitem, mc_bool_t *res);
/**
 * @brief add int32 into pack
 *
 * @param [in] ppack   : mc_pack_t*
 *		指向 pack 或 object/array 的指针
 * @param [in] name   : const char*
 *		字段名，ppack 为数组时 name 必须为 NULL
 * @param [in] value   : mc_int32_t
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/28 11:52:21
**/
int mc_pack_put_int32(mc_pack_t * ppack, const char *name, mc_int32_t value);

/**
 * @brief get int32 from pack
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 * @param [out] result   : mc_int32_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/28 15:30:37
**/
int mc_pack_get_int32(const mc_pack_t * ppack, const char *name, mc_int32_t * result);

/**
 * @brief add uint32 into pack
 *
 * @param [in] ppack   : mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 * @param [in] value   : mc_uint32_t
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/28 11:58:48
**/
int mc_pack_put_uint32(mc_pack_t * ppack, const char *name, mc_uint32_t value);

/**
 * @brief get uint32 from pack
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 object/array 的指针
 * @param [in] name   : const char*
 *		字段名，ppack 为数组时 name 必须为 NULL
 * @param [out] result   : mc_uint32_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/29 10:49:27
**/
int mc_pack_get_uint32(const mc_pack_t * ppack, const char *name, mc_uint32_t * result);

/**
 * @brief add uint64 into pack
 *
 * @param [in] ppack   : mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 *		字段名，ppack 为数组时 name 必须为 NULL
 * @param [in] value   : mc_int64_t
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/28 11:59:14
**/
int mc_pack_put_int64(mc_pack_t * ppack, const char *name, mc_int64_t value);

/**
 * @brief get int64 from pack
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 * @param [out] result   : mc_int64_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/29 10:58:34
**/
int mc_pack_get_int64(const mc_pack_t * ppack, const char *name, mc_int64_t * result);

/**
 * @brief add uint64 into pack
 *
 * @param [in] ppack   : mc_pack_t*
 *		指向 pack 或 object/array 的指针
 * @param [in] name   : const char*
 *		字段名，ppack 为数组时 name 必须为 NULL
 * @param [in] value   : mc_uint64_t
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/28 11:59:39
**/
int mc_pack_put_uint64(mc_pack_t * ppack, const char *name, mc_uint64_t value);

/**
 * @brief get uint64 from pack
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 * @param [out] result   : mc_uint64_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/29 11:07:05
**/
int mc_pack_get_uint64(const mc_pack_t * ppack, const char *name, mc_uint64_t * result);

/**
 * @brief add a string into pack
 *		向 pack 中加入字符串，会将结尾的 '\\0' 也加入
 *
 * @param [in] ppack   : mc_pack_t*
 *		指向 pack 或 object/array 的指针
 * @param [in] name   : const char*
 *		字段名，ppack 为数组时 name 必须为 NULL
 * @param [in] value   : const char*
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/28 13:51:48
**/
int mc_pack_put_str(mc_pack_t * ppack, const char *name, const char *value);

/**
 * @brief add a string into pack
 *		将 value 的前 n 个字节加入 pack 中，会自动补 '\\0'
 *
 * @param [in] ppack   : mc_pack_t*
 *		指向 pack 或 object/array 的指针
 * @param [in] name   : const char*
 *		字段名，ppack 为数组时 name 必须为 NULL
 * @param [in] value   : const char*
 * @param [in] len   : u_int
 *		长度
 * @return  int 
 * @retval   errno
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/28 18:22:29
**/
int mc_pack_put_nstr(mc_pack_t * ppack, const char *name, const char *value, u_int len);

/**
 * @brief get a string from pack
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 * @return  const char* 
 *		指向 string 的指针
 * @retval  MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/09 14:08:39
**/
const char *mc_pack_get_str(const mc_pack_t * ppack, const char *name);

/**
 * @brief get a string from pack with default value
 *		取得字符串，如果未找到，返回默认值
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 * @param [in] def_value   : const char*
 *		默认值，如果未找到，返回def_value
 * @return  const char* 
 *		指向 string 的指针
 * @retval  MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/09 14:10:26
**/
const char *mc_pack_get_str_def(const mc_pack_t * ppack, const char *name, const char *def_value);

/**
 * @brief add raw data into pack
 *
 * @param [in] ppack   : mc_pack_t*
 *		指向 pack 或 object/array 的指针
 * @param [in] name   : const char*
 *		字段名，ppack 为数组时 name 必须为 NULL
 * @param [in] rawdata   : const void*
 * @param [in] size   : u_int
 *		raw data 大小
 * @return  int 
 *		- 0 : succeed
 *		- <0 : failed
 * @retval errno   
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/02 21:40:01
**/
int mc_pack_put_raw(mc_pack_t * ppack, const char *name, const void *rawdata, u_int size);

/**
 * @brief get raw data from pack
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 pack 或 object 的指针
 * @param [in] name   : const char*
 * @param [out] size   : u_int*
 *		raw data 的大小
 * @return  const void* 
 *		指向 raw data 的指针
 * @retval   MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/09 14:12:40
**/
const void *mc_pack_get_raw(const mc_pack_t * ppack, const char *name, u_int *size);

/**
 * @brief get int32 from array
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 array 的指针
 * @param [in] arr_index   : u_int
 * @param [out] result   : mc_int32_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/31 15:58:55
**/
int mc_pack_get_int32_arr(const mc_pack_t * ppack, u_int arr_index, mc_int32_t * result);

/**
 * @brief get uint32 from array
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 array 的指针
 * @param [in] arr_index   : u_int
 * @param [out] result   : mc_uint32_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/31 16:00:42
**/
int mc_pack_get_uint32_arr(const mc_pack_t * ppack, u_int arr_index, mc_uint32_t * result);

/**
 * @brief get int64 from array
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 array 的指针
 * @param [in] arr_index   : u_int
 * @param [out] result   : mc_int64_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/31 16:01:48
**/
int mc_pack_get_int64_arr(const mc_pack_t * ppack, u_int arr_index, mc_int64_t * result);

/**
 * @brief get uint64 from array
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 array 的指针
 * @param [in] arr_index   : u_int
 * @param [out] result   : mc_uint64_t*
 *		结果
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2007/12/31 16:03:25
**/
int mc_pack_get_uint64_arr(const mc_pack_t * ppack, u_int arr_index, mc_uint64_t * result);

/**
 * @brief get string from array
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 array 的指针
 * @param [in] arr_index   : u_int
 * @return  const char* 
 *		指向 string 的指针
 * @retval   MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/09 14:14:51
**/
const char *mc_pack_get_str_arr(const mc_pack_t * ppack, u_int arr_index);

/**
 * @brief get string from array with default value
 *		用数组下标取得字符串，如果未找到，返回默认值
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 array 的指针
 * @param [in] arr_index   : u_int
 * @param [in] def_value   : const char*
 *		默认值，如果未找到，返回def_value
 * @return  const char* 
 *		指向 string 的指针
 * @retval  MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/09 14:15:56
**/
const char *mc_pack_get_str_arr_def(const mc_pack_t * ppack, u_int arr_index, const char *def_value);

/**
 * @brief get raw data from array
 *
 * @param [in] ppack   : const mc_pack_t*
 *		指向 array 的指针
 * @param [in] arr_index   : u_int
 * @param [out] size   : u_int*
 *		raw data 的大小
 * @return  const void* 
 *		指向 raw data 的指针
 * @retval  MC_PACK_PTR_ERR(retval)
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/09 14:17:41
**/
const void *mc_pack_get_raw_arr(const mc_pack_t * ppack, u_int arr_index, u_int *size);

/**
 * @brief get first item
 *
 * @param [in] ppack   : const mc_pack_t*
 * @param [out] pitem   : mc_pack_item_t*
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/05 21:34:26
**/
int mc_pack_first_item(const mc_pack_t * ppack, mc_pack_item_t * pitem);

/**
 * @brief get next item
 *
 * @param [in] pitem_now   : const mc_pack_item_t*
 * @param [out] pitem_out   : mc_pack_item_t*
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/05 21:35:17
**/
int mc_pack_next_item(const mc_pack_item_t * pitem_now, mc_pack_item_t * pitem_out);

/**
 * @brief get item by index
 *
 * @param [in] ppack   : const mc_pack_t*
 * @param [in] idx   : u_int
 * @param [out] pitem   : mc_pack_item_t*
 * @return  int 
 * @retval errno   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/05 21:35:44
**/
int mc_pack_get_by_idx(const mc_pack_t * ppack, u_int idx, mc_pack_item_t * pitem);

/**
 * @brief close a pack
 *		发送 pack 前必须调用此函数，将 pack 的剩余空间置为 0
 *
 * @param [in] ppack   : mc_pack_t*
 * @return  int 
 * @retval  errno
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/08 21:01:27
**/
int mc_pack_close(mc_pack_t * ppack);

/**
 * @brief reopen a pack
 *		重新打开一个 close 了的 pack
 *
 * @param [in] ppack   : mc_pack_t*
 * @param [in] buf_size   : u_int
 * @return  int 
 * @retval  errno
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/01/13 17:54:02
**/
int mc_pack_copy_item(const mc_pack_t * src_pack, mc_pack_t * dst_pack, const char *name);

/**
 * @brief copy item from one array to another
 *
 * @param [in] src_arr   : const mc_pack_t*
 *		source array
 * @param [in] dst_arr   : mc_pack_t*
 *		destination array
 * @param [in] idx   : u_int
 *		array index
 * @return  int 
 * @retval   errno
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/07 21:20:15
**/
int mc_pack_copy_item_arr(const mc_pack_t * src_arr, mc_pack_t * dst_arr, u_int idx);

/**
 * @brief get item by name
 *
 * @param [in] ppack   : const mc_pack_t*
 * @param [in] name   : const char*
 * @param [out] pitem   : mc_pack_item_t*
 * @return  int 
 * @retval   errno
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/02/13 11:54:16
**/
int mc_pack_get_item(const mc_pack_t * ppack, const char *name, mc_pack_item_t *pitem);

/**
 * @brief get item by array index
 *
 * @param [in] ppack   : const mc_pack_t*
 * @param [in] idx   : u_int
 * @param [out] pitem   : mc_pack_item_t*
 * @return  int 
 * @retval   errno
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/21 15:49:23
**/
int mc_pack_get_item_arr(const mc_pack_t * ppack, u_int idx, mc_pack_item_t *pitem);

/**
 * @brief callback function for mc_pack_foreach
 *
 * @param [in] pitem   : const mc_pack_item_t*
 *		one item in pack
 * @param [in] arg   : void*
 *		argument for callback
 * @return  int 
 * @retval   
 *		- <0 : stop and return to caller of foreach
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/17 17:37:12
**/
typedef int (*mc_pack_foreach_callback)(const mc_pack_item_t *pitem, void *arg);

/**
 * @brief for each item in pack do sth.
 *
 * @param [in] ppack   : const mc_pack_t*
 * @param [in] cb   : mc_pack_foreach_callback
 *		callback function
 * @param [in] arg   : void*
 *		argument for callback
 * @return  int 
 * @retval   
 *		- <0 : err returned by cb
 *		- >=0 : sum of retval of cb
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/17 17:36:48
**/
int mc_pack_foreach(const mc_pack_t *ppack, mc_pack_foreach_callback cb, void *arg);

/**
 * @brief get int32 from item
 *
 * @param [in] pitem   : const mc_pack_item_t*
 * @param [out] res   : mc_int32_t*
 * @return  int 
 * @retval   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/23 15:05:50
**/
int mc_pack_get_int32_from_item(const mc_pack_item_t *pitem, mc_int32_t *res);

/**
 * @brief get uint32 from item
 *
 * @param [in] pitem   : const mc_pack_item_t*
 * @param [out] res   : mc_uint32_t*
 * @return  int 
 * @retval   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/23 15:06:43
**/
int mc_pack_get_uint32_from_item(const mc_pack_item_t *pitem, mc_uint32_t *res);

/**
 * @brief get int64 from item
 *
 * @param [in] pitem   : const mc_pack_item_t*
 * @param [out] res   : mc_int64_t*
 * @return  int 
 * @retval   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/23 15:07:02
**/
int mc_pack_get_int64_from_item(const mc_pack_item_t *pitem, mc_int64_t *res);

/**
 * @brief get uint64 from item
 *
 * @param [in] pitem   : const mc_pack_item_t*
 * @param [out] res   : mc_uint64_t*
 * @return  int 
 * @retval   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 * @author zhouzr
 * @date 2008/04/23 15:07:20
**/
int mc_pack_get_uint64_from_item(const mc_pack_item_t *pitem, mc_uint64_t *res);

/**
 * @brief put item into pack
 *
 * @param [in] pack   : mc_pack_t*
 * @param [in] pitem   : const mc_pack_item_t*
 * @return  int 
 * @retval   
 *		- 0 : succeed
 *		- <0 : failed
 * @see 
 * @note 
 *	会取 item.key 的最末级名字，不自动处理子 pack 的情况
 * @author zhouzr
 * @date 2008/05/10 15:00:55
**/
int mc_pack_put_item(mc_pack_t *pack, const mc_pack_item_t *pitem);

/**
 * @brief 
 * 以写模式创建新的mcpack
 * @param [in/out] version   : int 创建的mcpack的版本
 * @param [in/out] buf   : char* 用于存储mcpack的缓冲区
 * @param [in/out] size   : int buf的缓冲区长度
 * @param [in/out] tempbuf   : char* 用于创建临时对象的缓冲区
 * @param [in/out] tmp_size   : int tempbuf的长度
 * @return  mc_pack_t* 创建的mcpack句柄
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2008/12/04 15:11:38
**/
mc_pack_t * mc_pack_open_w(int version, char * buf, int size, char * tempbuf, int tmp_size);

/**
 * @brief 
 * 以读模式打开已存在的mcpack
 * @param [in/out] buf   : char* 用于存储mcpack的缓冲区
 * @param [in/out] size   : int buf的缓冲区长度
 * @param [in/out] tempbuf   : char* 用于创建临时对象的缓冲区
 * @param [in/out] tmp_size   : int tempbuf的长度
 * @return  mc_pack_t* 创建的mcpack句柄
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2008/12/04 15:11:43
**/
mc_pack_t * mc_pack_open_r(const char * buf, int size, char * tempbuf, int tmp_size);

/**
 * @brief 
 * 以写模式打开已存在的mcpack
 * @param [in/out] buf   : char* 用于存储mcpack的缓冲区
 * @param [in/out] size   : int buf的缓冲区长度
 * @param [in/out] tempbuf   : char* 用于创建临时对象的缓冲区
 * @param [in/out] tmp_size   : int tempbuf的长度
 * @return  mc_pack_t* 创建的mcpack句柄
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2008/12/04 15:11:49
**/
mc_pack_t * mc_pack_open_rw(char * buf, int size, char * tempbuf, int tmp_size);

/**
 * @brief 
 * 获得mcpack的实际存储空间的内存地址
 * @param [in/out] pack   : mc_pack_t* 指定mcpack句柄
 * @return  void* 内存地址
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2008/12/04 15:11:51
**/
void * mc_pack_get_buffer(mc_pack_t * pack);

/**
 * @brief 
 * 从mcpack中删除字段(协议版本1不支持该功能)
 * @param [in/out] ppack   : mc_pack_t* 指定mcpack句柄
 * @param [in/out] name   : const char* 要删除的字段名称
 * @param [in/out] flag   : int 删除方法，1为标记删除，0为移动内存删除 
 * @return  int 0表示成功，非零失败
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2008/12/04 15:11:56
**/
int mc_pack_rm(mc_pack_t *ppack, const char *name, int flag);

/**
 * @brief 
 * 从数组中删除已存在的字段
 * @param [in/out] ppack   : mc_pack_t* 指定mcpack句柄
 * @param [in/out] arr_index   : u_int 要删除的字段序号
 * @param [in/out] flag   : int 删除方法，1为标记删除，0为移动内存删除 
 * @return  int 0表示成功，非零失败
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2008/12/04 15:11:59
**/
int mc_pack_rm_arr(mc_pack_t *ppack, u_int arr_index, int flag);


/**
 * @brief 
 * 从mc_pack_item中获取mcpack句柄
 * @param [in/out] pitem   : const mc_pack_item_t* MC_PT_OBJ类型的mcpack_item
 * @param [in/out] res   : mc_pack_t** 返回的mcpack句柄
 * @return  int 0表示成功，非零失败
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2008/12/04 15:12:18
**/
int mc_pack_get_pack_from_item(const mc_pack_item_t *pitem, const mc_pack_t **res);

/**
 * @brief 
 * 从mc_pack_item中获取数组的mcpack句柄
 * @param [in/out] pitem   : const mc_pack_item_t* MC_PT_ARR类型的mcpack_item
 * @param [in/out] res   : mc_pack_t** 返回的mcpack句柄
 * @return  int 0表示成功，非零失败
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2008/12/04 15:12:23
**/
int mc_pack_get_array_from_item(const mc_pack_item_t *pitem, const mc_pack_t **res);



/**
 * @brief 
 * 返回pack所使用的协议版本
 * @param [in/out] ppack   : const mc_pack_t*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/05 16:32:08
**/
int mc_pack_get_version(const mc_pack_t * ppack);


/**
 * @brief 
 * 修改int32字段
 * @param [in/out] ppack   : mc_pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] value   : mc_int32_t
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/05 16:34:00
**/
int mc_pack_mod_int32(mc_pack_t * ppack, const char *name, mc_int32_t value);

/**
 * @brief 
 * 修改数组中的int32字段
 * @param [in/out] ppack   : mc_pack_t*
 * @param [in/out] arr_index   : u_int
 * @param [in/out] value   : mc_int32_t*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/05 16:34:13
**/
int mc_pack_mod_int32_arr(mc_pack_t * ppack, u_int arr_index, mc_int32_t value);

/**
 * @brief 
 * 修改int64字段
 * @param [in/out] ppack   : mc_pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] value   : mc_int64_t
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/05 16:34:25
**/
int mc_pack_mod_int64(mc_pack_t * ppack, const char *name, mc_int64_t value);

/**
 * @brief 
 * 修改数组中的int64字段
 * @param [in/out] ppack   : mc_pack_t*
 * @param [in/out] arr_index   : u_int
 * @param [in/out] value   : mc_int64_t*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/05 16:34:32
**/
int mc_pack_mod_int64_arr(mc_pack_t * ppack, u_int arr_index, mc_int64_t value);

/**
 * @brief 
 * 修改uint32字段
 * @param [in/out] ppack   : mc_pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] value   : mc_uint32_t
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/05 16:34:46
**/
int mc_pack_mod_uint32(mc_pack_t * ppack, const char *name, mc_uint32_t value);

/**
 * @brief 
 * 修改数组中的uint32字段
 * @param [in/out] ppack   : mc_pack_t*
 * @param [in/out] arr_index   : u_int
 * @param [in/out] value   : mc_uint32_t*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/05 16:34:55
**/
int mc_pack_mod_uint32_arr(mc_pack_t * ppack, u_int arr_index, mc_uint32_t value);

/**
 * @brief 
 * 修改uint64字段
 * @param [in/out] ppack   : mc_pack_t*
 * @param [in/out] name   : const char*
 * @param [in/out] value   : mc_uint64_t
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/05 16:35:10
**/
int mc_pack_mod_uint64(mc_pack_t * ppack, const char *name, mc_uint64_t value);

/**
 * @brief 
 * 修改数组中的uint64字段
 * @param [in/out] ppack   : mc_pack_t*
 * @param [in/out] arr_index   : u_int
 * @param [in/out] value   : mc_uint64_t*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/05 16:35:22
**/
int mc_pack_mod_uint64_arr(mc_pack_t * ppack, u_int arr_index, mc_uint64_t value);

/**
 * @brief 
 * 删除字段
 * @param [in/out] ppack   : mc_pack_t*
 * @param [in/out] name   : const char*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/05 16:35:34
**/
int mc_pack_rm_item(mc_pack_t * ppack, const char * name, int flag);

/**
 * @brief 
 * 删除数组中的字段
 * @param [in/out] ppack   : mc_pack_t*
 * @param [in/out] arr_index   : u_int
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/05 16:35:44
**/
int mc_pack_rm_item_arr(mc_pack_t * ppack, u_int arr_index, int flag);

/**
 * @brief 
 * 清理使用完毕的pack，释放所占用的那部分临时缓冲区资源
 * @param [in/out] ppack   : mc_pack_t*
 * @return  void 
 * @retval   
 * @see 
 * @note 
 * @author yufan
 * @date 2009/02/10 16:33:44
**/
void mc_pack_finish(const mc_pack_t * ppack);

/**
 * @brief 
 * 根据mcpack的头部获取mcpack的总长度
 * @param [in/out] buf   : const char* 缓冲区大小
 * @param [in/out] size   : int 缓冲区长度
 * @return  int 
 * @retval   0 需要读取更多数据
 * 			 >0 mcpack的总长度
 * 			<0 出错
 * @see 
 * @note 
 * @author yufan
 * @date 2009/04/02 16:21:14
**/
int mc_pack_get_length_partial(const char * buf, unsigned int size);
#ifdef __cplusplus
}
#endif



#endif  //__MC_PACK_C_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
