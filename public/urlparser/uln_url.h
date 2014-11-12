/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: uln_url.h,v 1.10 2008/07/04 04:58:07 wangjg Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file uln_url.h
 * @author wangjg(com@baidu.com)
 * @version $Revision: 1.10 $ 
 *
 * @brief  提供url解析、合法性检查、归一化、合并url、提取域名主干、检查动态url等
 * 一系列与url相关的基本函数. 支持多种协议(http, https, ftp等等)及转义方式选择. 
 * 并支持ipv6地址格式的url
 *  
 **/


#ifndef  __ULN_URL_H_
#define  __ULN_URL_H_

#define ULN_URL_MAX_SCHEME_LEN            64
#define ULN_URL_MAX_USERINFO_LEN          64
#define ULN_URL_MAX_HOST_LEN              128
#define ULN_URL_MAX_PATH_LEN              1024
#define ULN_URL_MAX_QUERY_LEN             1024
#define ULN_URL_MAX_FRAGMENT_LEN          128

#define ULN_URL_MAX_URL_LEN               2048
#define ULN_MAX_PORT_NUM                  65535
#define ULN_HTTP_DEFAULT_PORT             80

/**
 *   URL : [scheme://][userinfo"@"]host[":"port]"/"path["?"query]["#"fragment]
 */

typedef struct _uln_url_t {
	char        scheme[ULN_URL_MAX_SCHEME_LEN];
	char        userinfo[ULN_URL_MAX_USERINFO_LEN];
	char        host[ULN_URL_MAX_HOST_LEN];
  	int         port;
	char        path[ULN_URL_MAX_PATH_LEN];
	char        query[ULN_URL_MAX_QUERY_LEN];
	char        fragment[ULN_URL_MAX_FRAGMENT_LEN];
} uln_url_t;

typedef enum _segment_type_t {
	/* [scheme://][userinfo"@"]host[":"port]"/"path["?"query]["#"fragment] */
	SEG_URL_SCHEME,            ///< [scheme]
	SEG_URL_USERINFO,          ///< [host]
	SEG_URL_HOST,              ///< [host]
	SEG_URL_PORT,              ///< [port]
	SEG_URL_PATH,              ///< [path]
	SEG_URL_QUERY,             ///< [query]
	SEG_URL_FRAGMENT,          ///< [fragment]

	SEG_URL_FILE,              ///< [path?query]
	SEG_URL_STATIC_PATH,       ///< [path]
	SEG_URL_MAIN_DOMAIN,       ///< etc, [baidu]
	SEG_URL_HOST_TRUNK,        ///< etc, [baidu]
} segment_type_t;

typedef enum _encode_mode_t {
	ULN_UN_CHANGE_MODE  = 0,   
	ULN_TO_ESCAPED_MODE = 1,   ///< char -> %xy
	ULN_UN_ESCAPED_MODE = 2,   ///< %xy  -> char
} encode_mode_t;

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief  解析url字符串, 各部分存入uln_url_t结构体内.
 * 
 * @param [in] url      : const char* 需解析的字符串
 * @param [out] p_url   : uln_url_t*  结构体指针
 *
 * @return  int  操作结果
 * @retval  0    成功
 * @retval  -1    失败 (不合法url, 参数不合法等)
 * @see  uln_check_url(const char*)
 * @note 
 *
 * 此函数首先严格按照RFC2396规范解析url字符串. 
 *
 * 如果url没严格遵循RFC，则内部对url作一份拷贝，并对拷贝作以下修改:
 * a) 去掉行首, 行末空格
 * b) 把危险字符,单双引号、空格、尖括号等，全部转义成%xy的形式
 * c) 去掉url中出现的所有\r\n
 * d) 把全部的"\"字符转换成"/"
 * e) 对于类似http:/host这样，缺少一个"/"的，补充一个"/" 
 * f) 对于类似http://www.baidu.com./path或http://www.baidu.com:/path，
 *    去掉多余的"."或":"
 * g) 对于http://www.baidu.com?query缺少"/",
 *    会补充成http://www.baidu.com/?query
 * 
 * 对修改后的url，再作一次严格的RFC检查.
 *
**/
int uln_parse_url(const char* url, uln_url_t* p_url);



/**
 * @brief 解析url字符串, 各部分存入uln_url_t结构体内.
 * 与uln_parse_url不同的是, 这里不再进行合法字符的检查. (相当于旧的url库ul_url.h里的ul_parse_url)
 * 适用于一些先前已经做过检查或normalize的字符串.
 * 不作合法性检查解析时有更高的性能, 但用户须保证输入的合法性.
 *
 * @param [in] url   : const char* 需解析的字符串
 * @param [out] p_url   : uln_url_t* 结构体指针
 * @return  int 操作结果
 * @retval  0 成功
 * @retval  -1 失败 (不合法url)
 * @see uln_parse_url, ul_parse_url
 * @note 
 * @author wangjg
 * @date 2008/06/30 04:38:31
**/
int uln_parse_url_without_check(const char* url, uln_url_t* p_url);


/**
 * @brief 归一化url. 
 *
 * @param [in]  p_url   : uln_url_t*  需要归一化的url结构体指针
 * @param [out] p_url   : uln_url_t*  原地保存归一化后的url
 * @return  int  操作结果
 * @retval  0    成功
 * @retval  -1    失败(归一化后的url过长或参数不合法等)
 *
 * @see  uln_normalize_url(const char*url, char* buf, u_int buf_len)
 * @note 
 *
 * 分别对url的各部分归一化.
 * a) 对scheme部分: 全部转换成小写
 * b) 对host部分  :
 *     1) 如果%xy转成的char是合法（如digit, alphabet等)，把它转成对应的char.
 *     2) 把host的大写字母全部转成小写.
 * c) 对path部分 :
 *     1) 把相对路径去掉, 如"/a/b/../c"->"/a/c"
 *     2) 把%xy->char或char->%xy, 视乎转义方式的选取. 
 *     3) 对于一些%xy编码的字母，如果是安全的，一律转成char形式.
 * d) 对query部分:
 *     同path 2), 3).
 * e) 对fragment部分:
 *     同path 2), 3).
 *
 * f) 其它:
 *    1) 在支持多协议模式下，如果scheme为空，会补充成"http"
 *    2) 在单协议模式下, 如果scheme中存在"http", 会清空. (为了兼容旧的ul_url.h库)
 *    3) 如果url是http协议(即scheme为空或"http")，且端口是80, 会把端口置为-1.
 *    4) 如果url是http协议(即scheme为空或"http"), 会把fragment置为空串
 *
**/
int uln_normalize_url(uln_url_t* p_url);



/**
 * @brief 把结构体形式的url转成字符串形式，输出到buffer中.
 *
 * @param [in]  p_url     : const uln_url_t* 结构体形式的url
 * @param [out] buf       : char*            输出buffer
 * @param [in]  buf_len   : u_int            buffer长度
 * @return  int  操作结果
 * @retval  0    成功
 * @retval  -1    失败
 * @see 
 * @note 
 * 
 * 此函数会原样输出url的各部分，并不对url语义作分析，例如,
 * 如果p_url->scheme为http, p_url->host为www.baidu.com, p_url->port = 80.
 * 会输出, http://www.baidu.com:80.
 * 即80端口还是会输出. 只有当p_url->port设为-1才不会输出其端口.
 *
**/
int uln_to_urlstr(const uln_url_t* p_url, char* buf, u_int buf_len);



/**
 * @brief 取出url的各个部分, 各部分名称在segment_type_t定义.
 *
 * @param [in] p_url           : const uln_url_t*  url
 * @param [in] segment_type    : segment_type_t   需获取的url字段
 * @param [out] buf            : char* 输出buffer
 * @param [in] buf_len         : u_int buffer长度
 * @return  int  操作结果
 * @retval  0    成功
 * @retval  -1    失败
 * @see  uln_set_url_segment
 * @note 
 * 要注意的是，SEG_URL_PATH，不包括query部分. 如果要获取包含query部分，请用:
 * SEG_URL_FILE
 *
 * 如果p_url->port == -1, 则SEG_URL_PORT时，取出的是一空字符串，即"".
**/
int uln_get_url_segment(const uln_url_t* p_url, segment_type_t segment_type, char* buf, u_int buf_len);



/**
 * @brief 指定或修改 url某个部分的内容.
 *
 * @param [in] p_url      : uln_url_t*    需要修改的url
 * @param [in] segment_type   : int           字段名
 * @param [in] buf            : const char*   需要指定的字段的具体内容
 * @return  int 操作结果
 * @retval  0   成功
 * @retval  -1   失败
 * @see  uln_get_url_segment
 * @note 
 * 
 * 为了保证set进去的部分的字符串是正确的，会对buf内容做合法性检查，如果
 * 检查不通过，此函数会内部做一个buf拷贝并修改之（同uln_parse_url所述），
 * 再检查并set进去.
 *
 * 如果SEG_URL_PORT时，传入的字符串为空串，表示设为默认端口，即把p_url->port置为-1.
**/
int uln_set_url_segment(uln_url_t* p_url, segment_type_t segment_type, const char* buf);



/**
 * @brief 通过base_url及一个相对路径的url, 合成一个新的url.
 * 新的url已被归一化.
 *
 * @param [in]  p_base_url       : const uln_url_t* base_url指针
 * @param [in]  child_url        : const char*      子url
 * @param [out] p_resolved_url   : uln_url_t*       解析合成后的url
 * @return  int 操作结果
 * @retval  0   成功 
 * @retval  -1   失败
 *
 * @see int uln_resolve_url(const uln_url_t*, const char*, char*, u_int);
 *
 * @note 
 * 如果child_url本身是一个完整的url，如以http开头等, 则会结果里保存的是
 * child_url的parse的结果.
**/
int uln_resolve_url(const uln_url_t* p_base_url, const char* child_url, uln_url_t* p_resolved_url);


/**
 * @brief 清空url里的内容
 *
 * @param [in] p_url   : uln_url_t*  待清空的url
 * @return  int  操作结果
 * @retval  0    成功
 * @retval  -1    失败(指针为空)
 * @see 
 * @note 
**/
int uln_clear_url(uln_url_t* p_url);

/**
 * @brief 检查是否一个动态url.
 *
 * @param [in] p_url   : const uln_url_t*
 * @return  int  操作结果
 * @retval  1    为动态url
 * @retval  0    非动态url
 * @retval  -1   不是一个合法url
 * @see  int uln_is_dyn_url(const char*)
 * @note 
 * 即检查一个url里query是否非空，或path里是否存在";"
**/
int uln_is_dyn_url(const uln_url_t* p_url);

/**
 * @brief 设置normalize时的字符转义方式.
 * 参数encode_mode为以下三种之一:
 *
 * ULN_UN_CHANGE_MODE : 不改变url的字符
 * ULN_UN_ESCAPED_MODE: %xy -> char
 * UNN_TO_ESCAPED_MODE: char-> %xy  (默认)
 *
 * @param [in] encode_mode : int ULN_UN_CHANGE_MODE | ULN_UN_ESCAPED_MODE | UNN_TO_ESCAPED_MODE
 * @return  int  操作结果
 * @retval  0    成功
 * @retval  -1    失败
 * @see 
 * @note 
 * 
 * 要注要的时，这里的转义只影响escape字符的表现方式，
 * 即ASCII值>=128的字符，是否以%xy的形式存在.
 *
 * 对于危险字符，如空格、引号、尖括号等，始终会转成以%xy形式存在.
 * 对于一些安全字符，如数字、字母及少量标点，如果原来以%xy存在，
 * 归一化后都会转成具体字符（不管encode_mode是ULN_UN_ESCAPED_MODE还是ULN_TO_ESCAPED_MODE). 
 * 当然ULN_UN_CHANGE_MODE这些字符都会保留原状
 *
**/
int uln_set_encode_mode(encode_mode_t encode_mode);

/**
 * @brief 设置是否支持多协议，如https, ftp等.
 * 若不调用此函数，默认为只支持http
 *
 * @param [in] is_multi_scheme : int 1 或 0，表示是否支持多协议
 * @return  int  操作结果
 * @retval  0    成功
 * @retval  -1    失败
 * @see 
 * @note 
 * 此函数会影响以下几种函数的调用:
 * 1. uln_parse_url/uln_check_url, 如果是单协议(http)下，其它协议会认为url非法,
 *    如ftp://nasdaq.baidu.com在单协议下不合法.
 *
 * 2. uln_normalize_url, 在多协议下，会保留"http"字样的scheme, 如果原来的scheme为空, 会补充"http"
 *    在单协议下，normalize后会去掉"http"的scheme.
 *
**/
int uln_set_multi_schemes(int is_multi_scheme);


/**
 * @brief 检查是否一个合法url.
 *
 * @param [in] url  : const char* 待检查的url
 * @return  int  操作结果
 * @retval  0    是合法url
 * @retval  -1    非合法url
 *
 * @see uln_parse_url(const char*, uln_url_t*);
 * @note 
 *  其策略同uln_parse_url.
**/
int uln_check_url(const char* url);
#ifdef __cplusplus
}
#endif



/**
 * @brief 归一化url. 
 * 这个函数对传入的url没有任何假设, 即如果这个url本身不合法，会返回0.
 *
 * @param [in]  url       : const char*
 * @param [out] buf       : char*
 * @param [in]  buf_len   : u_int
 * @return  int  操作结果
 * @retval  0    成功
 * @retval  -1    失败
 *
 * @see  uln_normalize_url(uln_url_t* p_url)
 * @note 
 * 1. 其策略同uln_normalize_url(uln_url_t* p_url)
 * 2. 可以作原地转换，如uln_normalize(url, url, buf_len).
 *    当归一化失败时，对原来的url并没有影响.
**/
int uln_normalize_url(const char* url, char* buf, u_int buf_len);



/**
 * @brief 取出url的特定部分.
 *
 * @param [in]  url            : const char*  传入的url
 * @param [in]  segment_type   : segment_type_t  需要取出的部分
 * @param [out] buf            : char*        输出的buffer
 * @param [in]  buf_len        : u_int        buffer长度
 *
 * @return  int  操作结果
 * @retval  0    成功
 * @retval  -1    失败
 *
 * @see uln_get_url_segment(uln_url_t*, int, char*, u_int)
 * @note 
 * 其策略同uln_get_url_segment(uln_url_t*, int, char*, u_int)
**/
int uln_get_url_segment(const char* url, segment_type_t segment_type, char* buf, u_int buf_len);

/**
 * @brief 判断一个url是否动态url.
 *
 * @param [in] url   : const char*  待判断的url
 * @return  int  操作结果
 * @retval  0    不是动态url
 * @retval  1    动态url
 * @retval  -1   非法url
 *
 * @see uln_is_dyn_url(uln_url_t*)
 * @note 
 * 其策略同uln_is_dyn_url(uln_url_t*)
**/
int uln_is_dyn_url(const char* url);

/**
 * @brief 通过base_url及一个相对路径的url, 合成一个新的url, 以字符串形式输出.
 * 新的url已被归一化.
 *
 * @param [in] p_base_url  : const uln_url_t*   base_url指针
 * @param [in] child_url   : const char*        子url
 * @param [out] buf        : char*              输出buffer
 * @param [in] buf_len     : u_int              输出的buffer长度
 * @return  int 操作结果
 * @retval  0   成功
 * @retval  -1  失败
 * @see int uln_resolve_url(const uln_url_t*, const char*, uln_url_t*);
 * @note 
 *
 * 如果child_url本身是一个完整的url，如以http开头等, 则会结果里保存的是
 * child_url的parse的结果.
**/
int uln_resolve_url(const uln_url_t* p_base_url, const char* child_url, char* buf, u_int buf_len);


#endif  //__ULN_URL_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
