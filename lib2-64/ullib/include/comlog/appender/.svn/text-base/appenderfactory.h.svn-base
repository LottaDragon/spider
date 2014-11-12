/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: appenderfactory.h,v 1.3 2009/07/03 06:08:44 zhangfang Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file appenderfactory.h
 * @author yingxiang(com@baidu.com)
 * @date 2008/09/04 18:05:08
 * @version $Revision: 1.3 $ 
 * @brief 
 *  
 **/


#ifndef  __APPENDERFACTORY_H_
#define  __APPENDERFACTORY_H_

#include "comlog/appender/appender.h"
#include "comlog/comlog.h"
#include "comlog/xutils.h"
namespace comspace{

class AppenderFactory{
public:
	//Actually, MAX_NAME_LEN is 15. A name with length >= 16 is invalid.
	static const unsigned int MAX_NAME_LEN = 16;
	//typedef Appender* (*func_type)(com_device_t&);

	static Appender* getAppender(com_device_t &dev);
	static Appender* tryAppender(com_device_t &dev);
	static int registAppender(const char *appenderName, func_type getAppender, func_type tryAppender);
	static bool isAppender(const char *appenderName);
	AppenderFactory();
	~AppenderFactory();
private:
	struct AppenderList{
		char name[MAX_NAME_LEN];
		func_type tryAppender;
		func_type getAppender;
		AppenderList* next;
	};

	static AppenderList* appList;
	static AppenderList* appTail;
	static pthread_mutex_t getLock;
	static pthread_mutex_t tryLock;

	static AppenderList* searchAppender(const char* appenderName);
	static void insertAppender(AppenderList* node);

};


};


#endif  //__APPENDERFACTORY_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
