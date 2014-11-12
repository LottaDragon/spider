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
 * @brief  �ṩurl�������Ϸ��Լ�顢��һ�����ϲ�url����ȡ�������ɡ���鶯̬url��
 * һϵ����url��صĻ�������. ֧�ֶ���Э��(http, https, ftp�ȵ�)��ת�巽ʽѡ��. 
 * ��֧��ipv6��ַ��ʽ��url
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
 * @brief  ����url�ַ���, �����ִ���uln_url_t�ṹ����.
 * 
 * @param [in] url      : const char* ��������ַ���
 * @param [out] p_url   : uln_url_t*  �ṹ��ָ��
 *
 * @return  int  �������
 * @retval  0    �ɹ�
 * @retval  -1    ʧ�� (���Ϸ�url, �������Ϸ���)
 * @see  uln_check_url(const char*)
 * @note 
 *
 * �˺��������ϸ���RFC2396�淶����url�ַ���. 
 *
 * ���urlû�ϸ���ѭRFC�����ڲ���url��һ�ݿ��������Կ����������޸�:
 * a) ȥ������, ��ĩ�ո�
 * b) ��Σ���ַ�,��˫���š��ո񡢼����ŵȣ�ȫ��ת���%xy����ʽ
 * c) ȥ��url�г��ֵ�����\r\n
 * d) ��ȫ����"\"�ַ�ת����"/"
 * e) ��������http:/host������ȱ��һ��"/"�ģ�����һ��"/" 
 * f) ��������http://www.baidu.com./path��http://www.baidu.com:/path��
 *    ȥ�������"."��":"
 * g) ����http://www.baidu.com?queryȱ��"/",
 *    �Ჹ���http://www.baidu.com/?query
 * 
 * ���޸ĺ��url������һ���ϸ��RFC���.
 *
**/
int uln_parse_url(const char* url, uln_url_t* p_url);



/**
 * @brief ����url�ַ���, �����ִ���uln_url_t�ṹ����.
 * ��uln_parse_url��ͬ����, ���ﲻ�ٽ��кϷ��ַ��ļ��. (�൱�ھɵ�url��ul_url.h���ul_parse_url)
 * ������һЩ��ǰ�Ѿ���������normalize���ַ���.
 * �����Ϸ��Լ�����ʱ�и��ߵ�����, ���û��뱣֤����ĺϷ���.
 *
 * @param [in] url   : const char* ��������ַ���
 * @param [out] p_url   : uln_url_t* �ṹ��ָ��
 * @return  int �������
 * @retval  0 �ɹ�
 * @retval  -1 ʧ�� (���Ϸ�url)
 * @see uln_parse_url, ul_parse_url
 * @note 
 * @author wangjg
 * @date 2008/06/30 04:38:31
**/
int uln_parse_url_without_check(const char* url, uln_url_t* p_url);


/**
 * @brief ��һ��url. 
 *
 * @param [in]  p_url   : uln_url_t*  ��Ҫ��һ����url�ṹ��ָ��
 * @param [out] p_url   : uln_url_t*  ԭ�ر����һ�����url
 * @return  int  �������
 * @retval  0    �ɹ�
 * @retval  -1    ʧ��(��һ�����url������������Ϸ���)
 *
 * @see  uln_normalize_url(const char*url, char* buf, u_int buf_len)
 * @note 
 *
 * �ֱ��url�ĸ����ֹ�һ��.
 * a) ��scheme����: ȫ��ת����Сд
 * b) ��host����  :
 *     1) ���%xyת�ɵ�char�ǺϷ�����digit, alphabet��)������ת�ɶ�Ӧ��char.
 *     2) ��host�Ĵ�д��ĸȫ��ת��Сд.
 * c) ��path���� :
 *     1) �����·��ȥ��, ��"/a/b/../c"->"/a/c"
 *     2) ��%xy->char��char->%xy, �Ӻ�ת�巽ʽ��ѡȡ. 
 *     3) ����һЩ%xy�������ĸ������ǰ�ȫ�ģ�һ��ת��char��ʽ.
 * d) ��query����:
 *     ͬpath 2), 3).
 * e) ��fragment����:
 *     ͬpath 2), 3).
 *
 * f) ����:
 *    1) ��֧�ֶ�Э��ģʽ�£����schemeΪ�գ��Ჹ���"http"
 *    2) �ڵ�Э��ģʽ��, ���scheme�д���"http", �����. (Ϊ�˼��ݾɵ�ul_url.h��)
 *    3) ���url��httpЭ��(��schemeΪ�ջ�"http")���Ҷ˿���80, ��Ѷ˿���Ϊ-1.
 *    4) ���url��httpЭ��(��schemeΪ�ջ�"http"), ���fragment��Ϊ�մ�
 *
**/
int uln_normalize_url(uln_url_t* p_url);



/**
 * @brief �ѽṹ����ʽ��urlת���ַ�����ʽ�������buffer��.
 *
 * @param [in]  p_url     : const uln_url_t* �ṹ����ʽ��url
 * @param [out] buf       : char*            ���buffer
 * @param [in]  buf_len   : u_int            buffer����
 * @return  int  �������
 * @retval  0    �ɹ�
 * @retval  -1    ʧ��
 * @see 
 * @note 
 * 
 * �˺�����ԭ�����url�ĸ����֣�������url����������������,
 * ���p_url->schemeΪhttp, p_url->hostΪwww.baidu.com, p_url->port = 80.
 * �����, http://www.baidu.com:80.
 * ��80�˿ڻ��ǻ����. ֻ�е�p_url->port��Ϊ-1�Ų��������˿�.
 *
**/
int uln_to_urlstr(const uln_url_t* p_url, char* buf, u_int buf_len);



/**
 * @brief ȡ��url�ĸ�������, ������������segment_type_t����.
 *
 * @param [in] p_url           : const uln_url_t*  url
 * @param [in] segment_type    : segment_type_t   ���ȡ��url�ֶ�
 * @param [out] buf            : char* ���buffer
 * @param [in] buf_len         : u_int buffer����
 * @return  int  �������
 * @retval  0    �ɹ�
 * @retval  -1    ʧ��
 * @see  uln_set_url_segment
 * @note 
 * Ҫע����ǣ�SEG_URL_PATH��������query����. ���Ҫ��ȡ����query���֣�����:
 * SEG_URL_FILE
 *
 * ���p_url->port == -1, ��SEG_URL_PORTʱ��ȡ������һ���ַ�������"".
**/
int uln_get_url_segment(const uln_url_t* p_url, segment_type_t segment_type, char* buf, u_int buf_len);



/**
 * @brief ָ�����޸� urlĳ�����ֵ�����.
 *
 * @param [in] p_url      : uln_url_t*    ��Ҫ�޸ĵ�url
 * @param [in] segment_type   : int           �ֶ���
 * @param [in] buf            : const char*   ��Ҫָ�����ֶεľ�������
 * @return  int �������
 * @retval  0   �ɹ�
 * @retval  -1   ʧ��
 * @see  uln_get_url_segment
 * @note 
 * 
 * Ϊ�˱�֤set��ȥ�Ĳ��ֵ��ַ�������ȷ�ģ����buf�������Ϸ��Լ�飬���
 * ��鲻ͨ�����˺������ڲ���һ��buf�������޸�֮��ͬuln_parse_url��������
 * �ټ�鲢set��ȥ.
 *
 * ���SEG_URL_PORTʱ��������ַ���Ϊ�մ�����ʾ��ΪĬ�϶˿ڣ�����p_url->port��Ϊ-1.
**/
int uln_set_url_segment(uln_url_t* p_url, segment_type_t segment_type, const char* buf);



/**
 * @brief ͨ��base_url��һ�����·����url, �ϳ�һ���µ�url.
 * �µ�url�ѱ���һ��.
 *
 * @param [in]  p_base_url       : const uln_url_t* base_urlָ��
 * @param [in]  child_url        : const char*      ��url
 * @param [out] p_resolved_url   : uln_url_t*       �����ϳɺ��url
 * @return  int �������
 * @retval  0   �ɹ� 
 * @retval  -1   ʧ��
 *
 * @see int uln_resolve_url(const uln_url_t*, const char*, char*, u_int);
 *
 * @note 
 * ���child_url������һ��������url������http��ͷ��, ������ﱣ�����
 * child_url��parse�Ľ��.
**/
int uln_resolve_url(const uln_url_t* p_base_url, const char* child_url, uln_url_t* p_resolved_url);


/**
 * @brief ���url�������
 *
 * @param [in] p_url   : uln_url_t*  ����յ�url
 * @return  int  �������
 * @retval  0    �ɹ�
 * @retval  -1    ʧ��(ָ��Ϊ��)
 * @see 
 * @note 
**/
int uln_clear_url(uln_url_t* p_url);

/**
 * @brief ����Ƿ�һ����̬url.
 *
 * @param [in] p_url   : const uln_url_t*
 * @return  int  �������
 * @retval  1    Ϊ��̬url
 * @retval  0    �Ƕ�̬url
 * @retval  -1   ����һ���Ϸ�url
 * @see  int uln_is_dyn_url(const char*)
 * @note 
 * �����һ��url��query�Ƿ�ǿգ���path���Ƿ����";"
**/
int uln_is_dyn_url(const uln_url_t* p_url);

/**
 * @brief ����normalizeʱ���ַ�ת�巽ʽ.
 * ����encode_modeΪ��������֮һ:
 *
 * ULN_UN_CHANGE_MODE : ���ı�url���ַ�
 * ULN_UN_ESCAPED_MODE: %xy -> char
 * UNN_TO_ESCAPED_MODE: char-> %xy  (Ĭ��)
 *
 * @param [in] encode_mode : int ULN_UN_CHANGE_MODE | ULN_UN_ESCAPED_MODE | UNN_TO_ESCAPED_MODE
 * @return  int  �������
 * @retval  0    �ɹ�
 * @retval  -1    ʧ��
 * @see 
 * @note 
 * 
 * ҪעҪ��ʱ�������ת��ֻӰ��escape�ַ��ı��ַ�ʽ��
 * ��ASCIIֵ>=128���ַ����Ƿ���%xy����ʽ����.
 *
 * ����Σ���ַ�����ո����š������ŵȣ�ʼ�ջ�ת����%xy��ʽ����.
 * ����һЩ��ȫ�ַ��������֡���ĸ��������㣬���ԭ����%xy���ڣ�
 * ��һ���󶼻�ת�ɾ����ַ�������encode_mode��ULN_UN_ESCAPED_MODE����ULN_TO_ESCAPED_MODE). 
 * ��ȻULN_UN_CHANGE_MODE��Щ�ַ����ᱣ��ԭ״
 *
**/
int uln_set_encode_mode(encode_mode_t encode_mode);

/**
 * @brief �����Ƿ�֧�ֶ�Э�飬��https, ftp��.
 * �������ô˺�����Ĭ��Ϊֻ֧��http
 *
 * @param [in] is_multi_scheme : int 1 �� 0����ʾ�Ƿ�֧�ֶ�Э��
 * @return  int  �������
 * @retval  0    �ɹ�
 * @retval  -1    ʧ��
 * @see 
 * @note 
 * �˺�����Ӱ�����¼��ֺ����ĵ���:
 * 1. uln_parse_url/uln_check_url, ����ǵ�Э��(http)�£�����Э�����Ϊurl�Ƿ�,
 *    ��ftp://nasdaq.baidu.com�ڵ�Э���²��Ϸ�.
 *
 * 2. uln_normalize_url, �ڶ�Э���£��ᱣ��"http"������scheme, ���ԭ����schemeΪ��, �Ჹ��"http"
 *    �ڵ�Э���£�normalize���ȥ��"http"��scheme.
 *
**/
int uln_set_multi_schemes(int is_multi_scheme);


/**
 * @brief ����Ƿ�һ���Ϸ�url.
 *
 * @param [in] url  : const char* ������url
 * @return  int  �������
 * @retval  0    �ǺϷ�url
 * @retval  -1    �ǺϷ�url
 *
 * @see uln_parse_url(const char*, uln_url_t*);
 * @note 
 *  �����ͬuln_parse_url.
**/
int uln_check_url(const char* url);
#ifdef __cplusplus
}
#endif



/**
 * @brief ��һ��url. 
 * ��������Դ����urlû���κμ���, ��������url�����Ϸ����᷵��0.
 *
 * @param [in]  url       : const char*
 * @param [out] buf       : char*
 * @param [in]  buf_len   : u_int
 * @return  int  �������
 * @retval  0    �ɹ�
 * @retval  -1    ʧ��
 *
 * @see  uln_normalize_url(uln_url_t* p_url)
 * @note 
 * 1. �����ͬuln_normalize_url(uln_url_t* p_url)
 * 2. ������ԭ��ת������uln_normalize(url, url, buf_len).
 *    ����һ��ʧ��ʱ����ԭ����url��û��Ӱ��.
**/
int uln_normalize_url(const char* url, char* buf, u_int buf_len);



/**
 * @brief ȡ��url���ض�����.
 *
 * @param [in]  url            : const char*  �����url
 * @param [in]  segment_type   : segment_type_t  ��Ҫȡ���Ĳ���
 * @param [out] buf            : char*        �����buffer
 * @param [in]  buf_len        : u_int        buffer����
 *
 * @return  int  �������
 * @retval  0    �ɹ�
 * @retval  -1    ʧ��
 *
 * @see uln_get_url_segment(uln_url_t*, int, char*, u_int)
 * @note 
 * �����ͬuln_get_url_segment(uln_url_t*, int, char*, u_int)
**/
int uln_get_url_segment(const char* url, segment_type_t segment_type, char* buf, u_int buf_len);

/**
 * @brief �ж�һ��url�Ƿ�̬url.
 *
 * @param [in] url   : const char*  ���жϵ�url
 * @return  int  �������
 * @retval  0    ���Ƕ�̬url
 * @retval  1    ��̬url
 * @retval  -1   �Ƿ�url
 *
 * @see uln_is_dyn_url(uln_url_t*)
 * @note 
 * �����ͬuln_is_dyn_url(uln_url_t*)
**/
int uln_is_dyn_url(const char* url);

/**
 * @brief ͨ��base_url��һ�����·����url, �ϳ�һ���µ�url, ���ַ�����ʽ���.
 * �µ�url�ѱ���һ��.
 *
 * @param [in] p_base_url  : const uln_url_t*   base_urlָ��
 * @param [in] child_url   : const char*        ��url
 * @param [out] buf        : char*              ���buffer
 * @param [in] buf_len     : u_int              �����buffer����
 * @return  int �������
 * @retval  0   �ɹ�
 * @retval  -1  ʧ��
 * @see int uln_resolve_url(const uln_url_t*, const char*, uln_url_t*);
 * @note 
 *
 * ���child_url������һ��������url������http��ͷ��, ������ﱣ�����
 * child_url��parse�Ľ��.
**/
int uln_resolve_url(const uln_url_t* p_base_url, const char* child_url, char* buf, u_int buf_len);


#endif  //__ULN_URL_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
