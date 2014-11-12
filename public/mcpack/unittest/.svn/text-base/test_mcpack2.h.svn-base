/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: test_mcpack2.h,v 1.1.2.2 2008/12/08 11:03:30 yufan Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file test_mcpack2.h
 * @author yufan(com@baidu.com)
 * @date 2008/11/18 15:18:27
 * @version $Revision: 1.1.2.2 $ 
 * @brief 
 *  
 **/


#ifndef  __TEST_MCPACK2_H_
#define  __TEST_MCPACK2_H_


#include <cxxtest/TestSuite.h>
#include "mc_pack.h"
//#include "mc_pack_api.h"
#include <string.h>
#include <stdio.h>
class mcpack_test : public CxxTest::TestSuite
{
public:
	void test_array()
	{
		char buf[1024];
		char tmpbuf[10240];
		memset(buf, 0xff, sizeof(buf));
		memset(tmpbuf, 0xff, sizeof(tmpbuf));
		int lll =10;	
		mc_pack_t *p;
		{
			p = mc_pack_open_w(2, buf, sizeof(buf), tmpbuf, sizeof(tmpbuf));
			mc_pack_t *p1 = mc_pack_put_array(p, "array");
			for(int pi=0; pi<lll; pi++)
			{
				mc_pack_t *p2 = mc_pack_put_object(p1, NULL);
				mc_pack_put_str(p2, "name", "bob");
				short sp1 = (short)pi+1;
				mc_pack_put_raw(p2, "point", &sp1, sizeof(short));
				mc_pack_put_int32(p2, "ladder", pi);
				double dp2 = 97.;
				mc_pack_put_raw(p2, "percent", &dp2, sizeof(double));
				mc_pack_close(p2);
			}
			mc_pack_close(p1);
			mc_pack_close(p);
		}
		//int size = mc_pack_get_size(p);
		{
			p = mc_pack_open_r(buf, sizeof(buf), tmpbuf, sizeof(tmpbuf));
			mc_pack_t *p1 = mc_pack_get_array(p, "array");
			int len = mc_pack_get_item_count(p1);
			TS_ASSERT_EQUALS(len, lll);
			for(int pi=0; pi<len; pi++)
			{
				//printf("repeat %d\n", pi);
				mc_pack_t *p2 = mc_pack_get_object_arr(p1, pi);
				TS_ASSERT(!MC_PACK_PTR_ERR(p2));
				int t;
				mc_pack_get_int32(p2, "ladder", &t);
				TS_ASSERT_EQUALS(t, pi);
				const char * sss = mc_pack_get_str(p2, "name");
				TS_ASSERT_SAME_DATA(sss, "bob", 4);
				unsigned int s;
				short *s2ss = (short *)mc_pack_get_raw(p2, "point", &s);
				TS_ASSERT_EQUALS(s, sizeof(short));
				TS_ASSERT_EQUALS(*s2ss, pi+1);
				double *dd = (double *)mc_pack_get_raw(p2, "percent", &s);
				TS_ASSERT_EQUALS(*dd, 97.)
			}
		}
	}
	void test_rm()
	{
		char buf[1024];
		char tmpbuf[10240];
		memset(buf, 0xff, sizeof(buf));
		memset(tmpbuf, 0xff, sizeof(tmpbuf));
		int lll =10;	
		mc_pack_t *p;
		{
			p = mc_pack_open_w(2, buf, sizeof(buf), tmpbuf, sizeof(tmpbuf));
			mc_pack_t *p1 = mc_pack_put_array(p, "array");
			for(int pi=0; pi<lll; pi++)
			{
				mc_pack_t *p2 = mc_pack_put_object(p1, NULL);
				mc_pack_put_str(p2, "name", "bob");
				short sp1 = (short)pi+1;
				mc_pack_put_raw(p2, "point", &sp1, sizeof(short));
				mc_pack_put_int32(p2, "ladder", pi);
				double dp2 = 97.;
				mc_pack_put_raw(p2, "percent", &dp2, sizeof(double));
				mc_pack_close(p2);
			}
			mc_pack_close(p1);
			mc_pack_close(p);
		}
		//int size = mc_pack_get_size(p);
		{
			p = mc_pack_open_rw(buf, sizeof(buf), tmpbuf, sizeof(tmpbuf));
			mc_pack_t *p1 = mc_pack_get_array(p, "array");
			int len = mc_pack_get_item_count(p1);
			TS_ASSERT_EQUALS(len, lll);
			for(int pi=0; pi<len-1; pi++)
			{
				mc_pack_rm_arr(p1, 0, 1);
			}
			len = mc_pack_get_item_count(p1);
			TS_ASSERT_EQUALS(len, 1);
			mc_pack_t *p2 = mc_pack_get_object_arr(p1, 0);
			TS_ASSERT(!MC_PACK_PTR_ERR(p2));
			int t;
			mc_pack_get_int32(p2, "ladder", &t);
			TS_ASSERT_EQUALS(t, lll-1);
		}
	}
	void test_create()
	{
		char buf[1024];
		char tmp[1024];
		memset(buf, 0xff, sizeof(buf));
		memset(tmp, 0xff, sizeof(tmp));
		{
			mc_pack_t * pack = mc_pack_open_w(2, buf, sizeof(buf), tmp, sizeof(tmp));
			mc_pack_put_int32(pack, "yufan", 1983);
			mc_pack_put_int32(pack, "xiaowei", 1985);
			mc_pack_put_str(pack,"dd", "kankanxingbuxing");
			mc_pack_t * subpack = mc_pack_put_object(pack, "baidu");
			mc_pack_put_int32(subpack, "yingxiang", 1989);
			mc_pack_put_int32(subpack, "nenghui", 1999);
			mc_pack_t * subarray = mc_pack_put_array(pack, "ibase");
			mc_pack_put_int32(subarray,0,2008);
			mc_pack_put_int32(subarray,0,2009);
			mc_pack_put_int32(subpack, "insertion", 9999);
			mc_pack_rm(subpack, "yingxiang", 1);
			mc_pack_put_int32(pack, "jiping", 1975);
			mc_pack_close(pack);
		}
		{
			mc_pack_t * pack = mc_pack_open_r(buf, sizeof(buf), tmp, sizeof(tmp));
			int t;
			mc_pack_get_int32(pack, "yufan", &t);
			TS_ASSERT_EQUALS(t, 1983);
			mc_pack_get_int32(pack, "xiaowei", &t);
			const char *s = mc_pack_get_str(pack, "dd");
			s += 1;
			mc_pack_t * subpack = mc_pack_get_object(pack, "baidu");
			TS_ASSERT(!MC_PACK_PTR_ERR(subpack));
			mc_pack_get_int32(subpack, "yingxiang", &t);
			mc_pack_get_int32(subpack, "nenghui", &t);
			mc_pack_get_int32(subpack, "insertion", &t);
			TS_ASSERT_EQUALS(t, 9999);
			mc_pack_t * subarray = mc_pack_get_array(pack, "ibase");
			TS_ASSERT(!MC_PACK_PTR_ERR(subarray));
			mc_pack_get_int32_arr(subarray,0,&t);
			TS_ASSERT_EQUALS(t, 2008);
			mc_pack_get_int32_arr(subarray,1,&t);
			mc_pack_get_int32(pack, "jiping", &t);
			TS_ASSERT_EQUALS(t, 1975);
		}
	}
	void test_item()
	{
		char buf[1024];
		char tmp[1024];
		memset(buf, 0xff, sizeof(buf));
		memset(tmp, 0xff, sizeof(tmp));
		{
			mc_pack_t * pack = mc_pack_open_w(2, buf, sizeof(buf), tmp, sizeof(tmp));
			mc_pack_put_int32(pack, "yufan", 1983);
			mc_pack_put_int32(pack, "xiaowei", 1985);
			mc_pack_put_str(pack,"dd", "kankanxingbuxing");
			mc_pack_t * subpack = mc_pack_put_object(pack, "baidu");
			mc_pack_put_int32(subpack, "yingxiang", 1989);
			mc_pack_put_int32(subpack, "nenghui", 1999);
			mc_pack_t * subarray = mc_pack_put_array(pack, "ibase");
			mc_pack_put_int32(subarray,0,2008);
			mc_pack_put_int32(subarray,0,2009);
			mc_pack_put_int32(subpack, "insertion", 9999);
			mc_pack_rm(subpack, "yingxiang", 1);
			mc_pack_put_int32(pack, "jiping", 1975);
			mc_pack_close(pack);
		}
		{
			mc_pack_t * pack = mc_pack_open_r(buf, sizeof(buf), tmp, sizeof(tmp));
			int t;
			mc_pack_item_t item;
			mc_pack_first_item(pack, &item);
			TS_ASSERT_SAME_DATA(item.key, "yufan", 6);
			mc_pack_get_int32_from_item(&item, &t);
			TS_ASSERT_EQUALS(t,1983);
		}
	}

	void test_2text()
	{
		char buf[1024];
		char tmp[1024];
		memset(buf, 0xff, sizeof(buf));
		memset(tmp, 0xff, sizeof(tmp));
		{
			mc_pack_t * pack = mc_pack_open_w(2, buf, sizeof(buf), tmp, sizeof(tmp));
			mc_pack_put_int32(pack, "yufan", 1983);
			mc_pack_put_int32(pack, "xiaowei", 1985);
			mc_pack_put_str(pack,"dd", "kankanxingbuxing");
			mc_pack_t * subpack = mc_pack_put_object(pack, "baidu");
			mc_pack_put_int32(subpack, "yingxiang", 1989);
			mc_pack_put_int32(subpack, "nenghui", 1999);
			mc_pack_t * subarray = mc_pack_put_array(pack, "ibase");
			mc_pack_put_int32(subarray,0,2008);
			mc_pack_put_int32(subarray,0,2009);
			mc_pack_put_int32(subpack, "insertion", 9999);
			mc_pack_rm(subpack, "yingxiang", 1);
			mc_pack_put_int32(pack, "jiping", 1975);
			mc_pack_close(pack);
		}
		char textbuf[1024];
		memset(textbuf, 0xff, sizeof(textbuf));
		{
			mc_pack_t * pack = mc_pack_open_r(buf, sizeof(buf), tmp, sizeof(tmp));
			mc_pack_pack2text(pack, textbuf, sizeof(textbuf), 0);
		}
		TS_ASSERT_EQUALS(strlen(textbuf), 185);
		memset(buf, 0xff, sizeof(buf));
		{
			mc_pack_t * pack = mc_pack_open_w(2, buf, sizeof(buf), tmp, sizeof(tmp));
			mc_pack_text2pack(textbuf, pack);
		}
		{
			char textbuf2[1024];
			mc_pack_t * pack = mc_pack_open_r(buf, sizeof(buf), tmp, sizeof(tmp));
			mc_pack_pack2text(pack, textbuf2, sizeof(textbuf2), 0);
			//puts(textbuf);
			TS_ASSERT_SAME_DATA(textbuf, textbuf2, strlen(textbuf) + 1);
		}
	}
	void test_putf()
	{
		char buf[8192], tmp[8192];
		mc_pack_t * pack = mc_pack_open_w(2, buf, sizeof(buf), tmp, sizeof(tmp));
		int ret=mc_pack_putf(pack, "cmd:%s", "SET");
		mc_pack_close(pack);
		char str[1024];
		mc_pack_pack2str(pack, str, sizeof(str));
		const char * ref = "cmd:SET ";
		TS_ASSERT_SAME_DATA(str, ref, strlen(ref) + 1);
	}
};













#endif  //__TEST_MCPACK2_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
