#ifndef __CCODE_H__
#define __CCODE_H__
#include "ul_iconv.h"

#define	TRBUF_SIZE	512

#define BIG5_NUM	0x3695	//13973

#define BIG5_1LO	0xa1	//161
#define	BIG5_1HI	0xf9	//249
#define BIG5_1NUM	0x59	//89
#define	BIG5_2LO1	0x40	//64
#define	BIG5_2HI1	0x7e	//126
#define	BIG5_2NUM1	0x3f	//63
#define	BIG5_2LO2	0xa1	//161
#define	BIG5_2HI2	0xfe	//254
#define	BIG5_2NUM2	0x5e	//94
#define	BIG5_2NUM	0x9d	//157

#define	GBK_NUM		0x5e02	//24066

#define GBK_1LO		0x81	//129
#define GBK_1HI		0xfe	//254
#define	GBK_1NUM	0x7e	//126
#define	GBK_2LO		0x40	//64
#define	GBK_2HI		0xfe	//254
#define	GBK_2NUM	0xbf	//191
#define GBK_2INV	0x7f	//127

#define CODETYPE_NONE -1
//mv to langtype
#define CODETYPE_ENG  0
//means gb18030 now
#define CODETYPE_GB   1
#define CODETYPE_BIG5 2
#define CODETYPE_UTF8 3
//rm
#define CODETYPE_JPKR 4

// added by jcode
#define CODETYPE_JIS  5
#define CODETYPE_SJIS 6
#define CODETYPE_EUCJP 7
//rm
#define CODETYPE_UTF8JP 8

//new
#define CODETYPE_ASCII 9
#define CODETYPE_ISO_8859_1 10
#define CODETYPE_EUCKR 11

//language type
#define LANGTYPE_NONE (-1)
#define LANGTYPE_ENGLISH 0
#define LANGTYPE_SIMP_CHINESE 1
#define LANGTYPE_TRAD_CHINESE 2
#define LANGTYPE_JAPANESE 3

//新旧ccode的标识，当LANGTYPE被置为此值时说明是旧ccode的识别结果
#define LANGTYPE_NULL (-3)

#define PY_MAXPAGESIZE 512000
#ifndef u_char
#define u_char		unsigned char
#endif

//============== for ccode_check ccode_check_content exit type =================
//modified to 19 from 23, to avoid redefinition with jcode
#define	MAX_CHECK_EXIT		19
#define	CCODE_CHECK_EXIT_1	1
#define	CCODE_CHECK_EXIT_2	2
#define	CCODE_CHECK_EXIT_3	3
#define	CCODE_CHECK_EXIT_4	4
#define	CCODE_CHECK_EXIT_5	5
#define	CCODE_CHECK_EXIT_6	6
#define	CCODE_CHECK_EXIT_7	7
#define	CCODE_CHECK_EXIT_8	8
#define	CCODE_CHECK_EXIT_9	9
#define	CCODE_CHECK_EXIT_10	10
#define	CCODE_CHECK_EXIT_11	11
#define	CCODE_CHECK_EXIT_12	12
#define	CCODE_CHECK_EXIT_13	13
#define	CCODE_CHECK_EXIT_14	14
#define	CCODE_CHECK_EXIT_15	15
#define	CCODE_CHECK_EXIT_16	16
#define	CCODE_CHECK_EXIT_17	17
#define	CCODE_CHECK_EXIT_18	18
#define	CCODE_CHECK_EXIT_19	19

#define CCODE_CHECK_EXIT_20	20
#define CCODE_CHECK_EXIT_21 21
#define CCODE_CHECK_EXIT_22 22
#define CCODE_CHECK_EXIT_23 23


#define	CCODE_CONT_URLNONE		101
#define	CCODE_CONT_SHORT		102
#define	CCODE_CONT_ENG2GB		103
#define	CCODE_CONT_GBBIG52ENG	104
#define	CCODE_CONT_UTF82NONE	105
#define	CCODE_CONT_NONE2ENG		106
#define	CCODE_CONT_REMAIN		107
#define	CCODE_CONT_UTF82ENG		108

#define CCODE_CONT_NONE2GB		109


/** 
 * @brief 基于iconv_t的编码转换结构体，通过ul_ccode_conv_open和ul_ccode_conv_close打开和关闭，非线程安全，各子线程需维护单独的结构
 */
typedef struct _ccode_conv_t{
	jp2gb_t jp2gb;
	gb2jp_t gb2jp;
}ccode_conv_t;

/*是否进行特殊简繁体转换的标志*/

//=========================================

/* You should specify the `path' to txt's files in which char-map tables are
 * allocated while using these functions.
 *
 * All tables are lying in path/code directory.
 */

/**
 * @brief 加载ccode data文件
 *
 * @param [in] path   : char* ata文件路径 eg: ./code
 * @param [in] flag   : int 是否进行特殊字符的转换，例如”瞭望“,默认不进行转换。
 * @return  int 
 * @retval  成功0 失败-1
 * @see 
 * @author chunq
 * @date 2007/12/12 10:32:09
**/
#define FLAG_SPECIAL_CHAR 1
#define FLAG_NOT_SPECIAL_CHAR 0 
int ul_ccode_load(char *path , int flag = FLAG_NOT_SPECIAL_CHAR) ;

/**
 * @brief	ccode_conv结构全局初始化. 
 *	在不需要加载ccode词典时，用这个接口替代ul_ccode_load()进行全局初始化。
 * @return  int 
 * @retval	0:初始化成功；-1:初始化失败。   
 * @see 
 * @author jiangling 
 * @date 2009/05/15 17:30:34
**/
int ul_ccode_conv_init();

/*
return codetype
currently: 
it can be 
0: english
1: GBK
2: Big5
3: UTF-8 encoded chinese
-1: Other
*/

int ul_ccode_check_realtext(const char* pstr);
int ul_ccode_check(char *pstr);
int ul_ccode_check_notetype (char *pstr, int *out_type);

/*
check whether the main content of a URL is Chinese and should be indexed
Notice: MUST call ul_trans2gb before sending pTitle & pContent to this function
		old_codetype is the return of ul_ccode_check
return: new codetype
*/
int ul_ccode_check_content
	(const char* pTitle, const char* pContent, const char* pUrl, int old_codetype);
int ul_ccode_check_content_notetype
	(const char* pTitle, const char* pContent, const char* pUrl, int old_codetype, int *out_type);

int ul_ccode_del();

/**
 * @brief ccode_conv结构全局回收函数。 
 *  在不需要用ul_ccode_load()加载ccode词典时，用这个接口替代ul_ccode_del()进行全局回收。
 * @return  void 
 * @author jiangling 
 * @date 2009/05/15 18:01:42
**/
void ul_ccode_conv_del();

 /*=============================================================================
  * Function:        ul_is_tradgbk_str       
  * 
  * Description:     judge whether a string is encoded in traditional gbk
  * 
  * Input:           const char* pstr, the string to be judged, it must be a
  * 				 
  * 				 zero ended string                
  * 
  * Return:          1, traditional gbk string; 0. simple gbk string 
  * 
  *============================================================================*/
int ul_is_tradgbk_str(const char* pstr);

/*
ul_trans2gb
return 0 when success
return -1 when error 
*/
int ul_trans2gb(char *input,char *output, int codetype);


 /*=============================================================================
  * Function:        ul_big5_gbk_str       
  * 
  * Description:     translate big5 encoding string into gbk encoding string
  * 
  * Input:           u_char* psrc, input big5 encoding string 
  *                  int len, the length of the psrc, len<=0, the the lenght 
  *                  will be calculated internally,  but must make sure that 
  *                  psrc ended with '\0';                
  * 
  * Output:          u_char* pDest, buffer to hold the gbk encoding string
  * 
  * Return:          <0, failed; >=0, succeed. the lengh of the input string; 
  * 
  * Others:          the buffer length of pdes must be larger or equal than the length
  * 				 of input string.  ul_ccode_load() must be called before
  * 				 this function.
  *============================================================================*/
int ul_big5_gbk_str(u_char *psrc, u_char *pdes, int len);

int ul_big5_gbi_str(u_char *psrc, u_char *pdes, int len);

 /*=============================================================================
  * Function:        ul_gbk_big5_str       
  * 
  * Description:     translate gbk encoding string into big5 encoding string
  * 
  * Input:           u_char* psrc, input big5 encoding string 
  *                  int len, the length of the psrc, len<=0, the the lenght 
  *                  will be calculated internally,  but must make sure that 
  *                  psrc ended with '\0';                
  * 
  * Output:          u_char* pDest, buffer to hold the gbk encoding string
  * 
  * Return:          <0, failed; >=0, succeed. the lengh of the input string; 
  * 
  * Others:          the buffer length of pdes must be larger or equal than the length
  * 				 of input string.  ul_ccode_load() must be called before
  * 				 this function.
  *============================================================================*/
int ul_gbk_big5_str(u_char *psrc, u_char *pdes, int len);

int ul_gbk_gbi_str(u_char *psrc, u_char *pdes, int len);
int ul_gbi_big5_str(u_char *psrc, u_char *pdes, int len);
int ul_gbi_gbk_str(u_char *psrc, u_char *pdes, int len);


// transfer quanjiao keyboard letter/number/symbol to banjiao 
// this transformation can be made in-place, i.e. szIn == szOut
void ul_trans2bj(const char *szIn, char *szOut);

// transfer some special chinese quanjiao symbols to its corresponding banjiao form
// this transformation can be made in-place, i.e. szIn == szOut
void ul_trans2bj_ext(const char *szIn, char *szOut);

// transfer from uppercase to lower case, for gbk-encoded string
// this transformation can be made in-place, i.e. szIn == szOut
void ul_trans2lower(const char *szIn, char *szOut);

/*
obsolete function, use ul_trans2bj/ul_trans2bj_ext/ul_trans2lower instead
* transfer quanjiao to banjiao and upper to lower

int ul_transfer_ch(char *input,char *output);
*/

/*=============================================================================
 * Function:		ul_gbk_to_utf8       
 *
 * Description:		translate gbk encoding string into utf8 encoding string
 *
 * Input:			const char* pSrc, input gbk encoding string 
 * 					const unsigned nDesCnt, the length of the dest buffer
 *
 * Output:			char* pDest, buffer to hold the utf8 encoding string
 *
 * Return:			<0, failed; >0, succeed. the lengh of the utf8 string; 
 *
 * Others:			nDestCnt must be larger than 3/2*strlen(pSrc);
 *
 *============================================================================*/
int ul_gbk_to_utf8(char* pDest, const unsigned nDesCnt, const char* pSrc);

/*=============================================================================
 * Function:		ul_utf8_to_gbk       
 *
 * Description:		translate utf8 encoding string into gbk encoding string
 *
 * Input:			const char* pSrc, input gbk encoding string 
 * 					const unsigned nDesCnt, the length of the dest buffer
 *
 * Output:			char* pDest, buffer to hold the gbk encoding string
 *
 * Return:			<0, failed; >0, succeed. the lengh of the gbk string; 
 *
 * Others:			nDestCnt must be larger than strlen(pSrc);
 *
 *============================================================================*/
int ul_utf8_to_gbk(char* pDest, const unsigned nDesDnt, const char* pSrc);

inline int is_big5( u_char *upstr )
{
	if ( (upstr[0]>=0xa1) && (upstr[0]<=0xf9) &&
	     ( (upstr[1]>=0x40) && (upstr[1]<=0x7e) ||
               (upstr[1]>=0xa1) && (upstr[1]<=0xfe)
	   ) ) {
		return 1;
	}
	else{
		return 0;
	}
}

inline int is_gbk( u_char *pstr )
{
	if ( (pstr[0]>=GBK_1LO) && (pstr[0]<=GBK_1HI) &&
	     (pstr[1]>=GBK_2LO) && (pstr[1]<=GBK_2HI) &&
             (pstr[1]!=GBK_2INV)
	   ) {
		return 1;
	}
	else{
		return 0;
	}
}

inline int is_euckr(u_char* pstr){
	return (pstr[0] >= 0xA1 && pstr[0] <= 0xFE && pstr[1] >= 0xA1 && pstr[1] <= 0xFE) ? 1 : 0;
}


inline int is_gb(u_char* pstr)
{
	if ( (pstr[0]>=0xB0) && (pstr[0]<=0xF7) &&
	     (pstr[1]>=0xA1) && (pstr[1]<=0xFE) ) 
	{
		return 1;
	}
	else{
		return 0;
	}
}

inline int is_big5_str(const char* pstr)
{
	char szContent[1024];
	char* pSrc = (char*) pstr;
	char* pDest = szContent;
	char* pEnd = szContent + 256;
	u_char ch;
	int count = 0, len, i, real_count=0;
	short former_word = 0;

	// Get out the double bite words
	while( (ch = *pSrc) != 0 && pSrc[1] ){
		if ( ch <= 0x7f )
			++pSrc;
		else{
			if(*(short*)pSrc != former_word)
				++real_count;
			*pDest++ = ch;
			*pDest++ = pSrc[1];
			former_word= *(short*)pSrc;
			pSrc += 2;
			++count;
		}
        if( pDest >= pEnd )
		   break;
	}
	*pDest = 0;
	len = pDest - szContent;

	if(count == 0)
		return 0;

	for(i= 0;i<count; i++){
		if(!is_big5((u_char*)&szContent[i]))
			return 0;
		if(!is_gb((u_char*)&szContent[i]))
			return 1;
	}

	// can not judge from code range, make a choice of bigger probability
	return 0;
}

inline int is_gbk_str(const char* pSrc);



/**
 * @brief 初始化中日文编码转换所需的结构ccode_conv_t
 * <p>该结构不是线程安全的，每个子线程需要单独维护一个</p>
 *
 * @param conv 待初始化的转换结构体，不能为NULL
 *
 * @return 成功返回0，出错返回-1
 */
int ul_ccode_conv_open(ccode_conv_t* conv);

/**
 * @brief	线程内ccode初始化函数，每个线程调用一次. 
 * 等同于ul_ccode_conv_open(ccode_conv_t* conv)，但不需要显式传递ccode_conv_t结构.
 * @author	jiangling 
 * @date 2009/05/15 10:13:10
**/
int ul_ccode_conv_open();

/**
 * @brief 关闭中日文编码转换所需的结构
 * <p>该不是线程安全的，每个子线程需要单独维护一个</p>
 *
 * @param conv 待关闭的转换结构体
 *
 * @return 返回0
 */
int ul_ccode_conv_close(ccode_conv_t* conv);

/**
 * @brief	线程内ccode关闭函数.
 * 等同于ul_ccode_conv_close(ccode_conv_t* conv)，但不需要显式传递ccode_conv_t结构.
 * @author	jiangling 
 * @date 2009/05/15 10:12:23
**/
int ul_ccode_conv_close();

/** 
 * @brief 将输入字符串转换成GBI编码
 * <p>目前处理GB18030(GBK) BIG5 UTF8到GBI的转换。其它输入编码类型将直接拷贝</p>
 * 
 * @param input 输入字符串
 * @param output 输出buffer，注意：如果输入编码类型是UTF8，则input和output不可是同一buffer，output大小最好是input的2倍
 * @param outlen 输出buffer大小，当输出buffer写满时将不再继续转换
 * @param codetype 输入的编码类型
 * @param conv 用于做编码转换的结构体，用ul_ccode_conv_open和ul_ccode_conv_close打开和关闭。该结构不是线程安全的，各子线程需要维护单独的结构
 * 
 * @return 转换成功返回0，出错返回-1
 */
int ul_trans2gbi(char *input, char *output, int outlen, int codetype, ccode_conv_t* conv);

int ul_trans2gbi(char *input, char *output, int outlen, int codetype);

/** 
 * @brief 将输入字符串转换成GB18030编码
 * <p>目前处理BIG5 UTF8 JIS SHIFT_JIS EUCJP到GB18030的转换，其它输入编码类型将直接拷贝</p>
 * 
 * @param input 输入字符串
 * @param output 输出buffer，注意：如果输入编码类型是UTF8 JIS SHIFT_JIS EUCJP，则input和output不可是同一buffer，output的大小最好是input的2倍
 * @param outlen 输出buffer大小，当输出buffer写满时将不再继续转换
 * @param codetype 输入的编码类型
 * @param cconv 用于做编码转换的结构体，用ul_ccode_conv_open和ul_ccode_conv_close打开和关闭。该结构不是线程安全的，各子线程需要维护单独的结构
 * 
 * @return 转换成功返回0，出错返回-1
 */
int ul_trans2gb18030(char* input, char* output, int outlen, int codetype, ccode_conv_t* conv);

int ul_trans2gb18030(char* input, char* output, int outlen, int codetype);

/**
 * @brief 判断字符串前4个字节是否在gb18030码域内
 *
 * @param s 输入字符串，可以为NULL
 *
 * @return 是则返回true，NULL或不在则返回false
 */
inline bool ul_is_gb18030_4bytes(const char* s){
	return (s != NULL)
		&& IN_RANGE(s[0], 0x81, 0xFE)
		&& IN_RANGE(s[1], 0x30, 0x39)
		&& IN_RANGE(s[2], 0x81, 0xFE)
		&& IN_RANGE(s[3], 0x30, 0x39);
}

/**
 * @brief 判断字符串前4个字节是否是gb18030-2005的四字节汉字
 *
 * @param s 输入字符串，可以为NULL
 *
 * @return 是则返回true，NULL或不是则返回false
 */
inline bool ul_is_gb18030_4bytes_hanzi(const char* s){
	return (s != NULL)
		&& (IN_RANGE(s[0], 0x81, 0x82) || IN_RANGE(s[0], 0x95, 0x98)) //2 sections
		&& IN_RANGE(s[1], 0x30, 0x39)
		&& IN_RANGE(s[2], 0x81, 0xFE)
		&& IN_RANGE(s[3], 0x30, 0x39);
}

/** 
 * @brief 判断字符串开头是否是合法的GB18030字符，给出字符的字节数
 * 
 * @param s 输入字符串，假定是gb18030编码
 * 
 * @return 字符的字节数
 * 0: 如果输入为NULL或空串
 * 1: 如果输入首字节为ASCII字符
 * 2: 如果输入首2字节为GBK字符，即GB18030的双字节部分
 * 4: 如果输入首4字节为GB18030的四字节部分
 * -1: 如果输入不是合法GB18030字符
 */
int ul_next_gb18030(const char* s);

/** 
 * @brief 判断字符串含GB18030字符个数，包括ascii字符、GB18030双字节、GB18030四字节。不包括非法字符
 * 
 * @param s 输入字符串，假定是GB18030编码
 * 
 * @return GB18030字符个数
 */
int ul_gb18030_len(const char* s);

/** 
 * @brief 判断字符串首两字节是否是GB全角标点
 * <p>包括： 、 。 · ˉ ˇ ¨ 〃 — ～ ‖ … ‘ ’ “ ” 〔 〕〈 〉 《 》 「 」 『 』 〖 〗【 】
 *  ！ ＂ ＃ ￥ ％ ＆ ＇ （ ）＊ ＋ ， － ． ／： ； ＜ ＝ ＞ ？ ［ ＼］ ＾ ＿ ｀｛ ｜ ｝ ￣</p>
 * 
 * @param s 输入字符串，假定是GB18030编码
 * 
 * @return 是返回true，否则返回false
 */
inline bool ul_is_gb_punct(const char* s){
	return (s != NULL) && (((u_char)s[0] == 0xA1 && IN_RANGE((u_char)s[1], 0xA2, 0xBF) && (u_char)s[1] != 0xA9) //punctuation in A1
			|| (IS_GB_SMAEK((u_char*)s) && !IS_GB_1234((u_char*)s) && !IS_GB_ABCD((u_char*)s)));//punctuation in A3
}

/**
 * @brief 初始化gb18030到utf8间的转换所需的iconv_t结构
 * <p>iconv不是线程安全的，每个子线程需要单独维护一个iconv_t结构</p>
 *
 * @param gb18030_utf8 gb18030到utf8的转换结构，如为NULL则不初始化
 * @param utf8_gb18030 utf8到gb18030的转换结构，如为NULL则不初始化
 *
 * @return 成功返回0，出错返回-1
 */
int ul_gb18030_utf8_open(iconv_t* gb18030_utf8, iconv_t* utf8_gb18030);

/**
 * @brief 关闭gb18030到utf8间的转换所需的iconv_t结构
 * <p>iconv不是线程安全的，每个子线程需要单独维护一个iconv_t结构</p>
 *
 * @param gb18030_utf8 gb18030到utf8的转换结构，如为NULL则不处理
 * @param utf8_gb18030 utf8到gb18030的转换结构，如为NULL则不处理
 *
 * @return 返回0
 */
int ul_gb18030_utf8_close(iconv_t* gb18030_utf8, iconv_t* utf8_gb18030);

/** 
 * @brief 将输入的gb18030编码的字符串转换为big5编码
 * <p>对于big5没有的gb18030字符，以两个空格代替。</p>
 * 
 * @param psrc 输入字符串
 * @param pdes 输出字符串，两者可以是同一个buffer，否则请保证输出buffer大小至少跟输入相同
 * @param slen 输入字符串长度，函数至多转换slen个字节，如slen<=0，将转换整个输入
 * 
 * @return 转换成功返回1，出错返回-1
 */
int ul_gb18030_big5_str(u_char* psrc, u_char* pdes, int slen);

/** 
 * @brief 将输入的big5编码的字符串转换成gb18030编码
 * <p>由于big5字符在gbk中都有映射，而gbk是gb18030的子集，所以本函数实际做的是big5->gbk转换</p>
 * 
 * @param psrc 输入字符串
 * @param pdes 输出字符串，两者可以是同一个buffer，否则请保证输出buffer大小至少跟输入相同
 * @param slen 输入字符串长度，函数至多转换slen个字节，如slen<=0，将转换整个输入
 * 
 * @return 转换成功返回1，出错返回-1
 */
int ul_big5_gb18030_str(u_char* psrc, u_char* pdes, int slen);

/** 
 * @brief 将输入的gb18030编码的字符串转换为简体
 * 
 * @param psrc 输入字符串
 * @param pdes 输出字符串，两者可以是同一个buffer，否则请保证输出buffer大小至少跟输入相同
 * @param slen 输入字符串长度，函数至多转换slen个字节，如slen<=0，将转换整个输入
 * 
 * @return 转换成功返回1，出错返回-1
 */
int ul_gb18030_gbi_str(u_char* psrc, u_char* pdes, int slen);

/** 
 * @brief 将输入的gbi编码的字符串转换成gb18030编码
 * 
 * @param psrc 输入字符串
 * @param pdes 输出字符串，两者可以是同一个buffer，否则请保证输出buffer大小至少跟输入相同
 * @param slen 输入字符串长度，函数至多转换slen个字节，如slen<=0，将转换整个输入
 * 
 * @return 转换成功返回1，出错返回-1
 */
int ul_gbi_gb18030_str(u_char* psrc, u_char* pdes, int slen);

/** 
 * @brief 将gb18030编码的字符串转换为utf8编码
 * <p>遇到转换失败的字符用一个空格代替</p>
 * 
 * @param psrc 输入字符串
 * @param pdes 输出字符串，不可与输入字符串相同，其大小不应小于输入的3/2
 * @param destlen 输出buffer大小，当buffer写满时转换将停止
 * @param gb18030_utf8 对应的iconv_t结构，用ul_gb18030_utf8_open打开，用ul_gb18030_utf8_close关闭，非线程安全
 * 
 * @return 成功返回转换失败的字符数，出错返回-1
 */
int ul_gb18030_utf8_str(char* psrc, char* pdes, int destlen, iconv_t* gb18030_utf8);

int ul_gb18030_utf8_str(char* psrc, char* pdes, int destlen);

/** 
 * @brief 将utf8编码的字符串转换为gb18030编码
 * <p>遇到转换失败的字符时，如果是单字节用一个空格代替，否则用两个空格代替</p>
 * 
 * @param psrc 输入字符串
 * @param pdes 输出字符串，不可与输入字符串相同，其大小不应小于输入的2倍
 * @param destlen 输出buffer大小，当buffer写满时转换将停止
 * @param utf8_gb18030 对应的iconv_t结构，用ul_gb18030_utf8_open打开，用ul_gb18030_utf8_close关闭，非线程安全
 * 
 * @return 成功返回转换失败的字符数，出错返回-1
 */
int ul_utf8_gb18030_str(char* psrc, char* pdes, int destlen, iconv_t* utf8_gb18030);

int ul_utf8_gb18030_str(char* psrc, char* pdes, int destlen);

/** 
 * @brief 编码和语言识别接口
 * 
 * @param page 页面字符串，不可为NULL
 * @param url 页面url，不可为NULL
 * @param charset 页面charset字符串，如为NULL则将在page中查找
 * @param content 页面正文，可为NULL
 * @param[out] codetype 编码类型
 * @param[out] langtype 语言类型
 * 
 * @return 识别成功返回1，出错返回-1
 */
int ul_ccode_detect(const char* page, const char* url, const char* charset, const char* content, int* codetype, int* langtype);

/**
 * @brief 日文编码和语言识别接口，直接封装ul_ccode_detect
 *
 * @param page 页面字符串，不可为NULL
 * @param url 页面url，不可为NULL
 * @param charset 页面charset字符串，如为NULL则将在page中查找
 * @param content 页面正文，可为NULL
 * @param[out] out_type 编码类型
 *
 * @return 识别成功返回1，出错返回-1
 */
int ul_jcode_detect(char *page, const char* url, const char* charset, const char* content, int *out_type);

/**
 * @brief	获取线程内全局私有的ccode_conv结构 
 * @author jiangling 
 * @date 2009/05/15 10:21:52
**/
ccode_conv_t *get_ccode_conv();

#endif
