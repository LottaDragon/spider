#include <pcre.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spidns_robots.h"
#include <ul_log.h>

inline int transform_pattern( char *ori_str , int ori_len , char *pattern_buffer , int bufferlen )
{	
	int pat_len = 0;
	char* ori_ptr = ori_str;
	char* ori_end = ori_str + ori_len;

	pattern_buffer[pat_len++] = '^';

	if (*ori_ptr == '^')
	{
		++ori_ptr;
	}
	
	for (; ori_ptr != ori_end && pat_len < bufferlen - 1; ++ori_ptr)
	{
		switch (*ori_ptr)
		{
			case '*':
				pattern_buffer[pat_len++] = '.';
				pattern_buffer[pat_len++] = '*';
				break;
			case '?':
				pattern_buffer[pat_len++] = '\\';
				pattern_buffer[pat_len++] = '?';
				break;
			case '.':
				pattern_buffer[pat_len++] = '\\';
				pattern_buffer[pat_len++] = '.';
				break;
			default:
				pattern_buffer[pat_len++] = *ori_ptr;
		}
	}
	pattern_buffer[pat_len] = '\0';
	if (ori_ptr != ori_end)
	{
		ul_writelog(UL_LOG_WARNING, "[spidns_robots.cpp: transform_pattern] robots %s exceed pattern buffer length.", ori_str);
		return -1;
	}
	return 0; 
}



#define MAX_ROBOTS_PATTERN_LENGTH 4096

int 
parse_robot( char *robostr , int len , robot_t &robot )
{
	char pattern[ MAX_ROBOTS_PATTERN_LENGTH ];
	int erroffset;
	const char *error;
	
	if( strlen( robostr ) < 2 )
	{
		return -1;	
	}

	switch( robostr[0] )
	{	
		case STR_ALLOW_COMMON:
			
	//		printf("Common Allow Len: %s %d\n",robostr , len);
			robot.type = ROBOTS_ALLOW_COMMON;
			robot.ptr = ( unsigned char *)malloc((size_t)len);
			if(robot.ptr == NULL )
			{
				return -2;
			}
			strncpy( (char*)robot.ptr , robostr+1 , (size_t)len);
			
		break;
		case STR_DISALLOW_COMMON:
			
	//		printf("Common Disallow Len: %s %d\n",robostr , len);
			robot.type = ROBOTS_DISALLOW_COMMON;
			robot.ptr = ( unsigned char *)malloc((size_t)len);
			if(robot.ptr == NULL )
			{
					return -2;
			}
			strncpy( (char*)robot.ptr , robostr+1 , (size_t)len);
			
		break;
		case STR_ALLOW_TEXT:
			
	//		printf("Text Allow Len: %s %d\n",robostr , len);
			robot.type = ROBOTS_ALLOW_TEXT;
			robot.ptr = ( unsigned char *)malloc((size_t)len);
			if(robot.ptr == NULL )
			{
				return -2;
			}
			strncpy( (char*)robot.ptr , robostr+1 , (size_t)len);
			
		break;
		case STR_DISALLOW_TEXT:
			
	//		printf("Text Disallow Len: %s %d\n",robostr , len);
			robot.type = ROBOTS_DISALLOW_TEXT;
			robot.ptr = ( unsigned char *)malloc((size_t)len);
			if(robot.ptr == NULL )
                        {
                                return -2;
                        }
                        strncpy( (char*)robot.ptr , robostr+1 , (size_t)len);
		break;
		case STR_ALLOW_REGEX:
			//printf("Reg Allow: %s\n",robostr);
			robot.type = ROBOTS_ALLOW_REGEX;
			
			if (transform_pattern( robostr+1 , len - 1 , pattern , MAX_ROBOTS_PATTERN_LENGTH ) != 0)
			{
				return -4;
			}
            //printf("Reg Allow: %s\n", pattern );

			robot.ptr = (unsigned char *)pcre_compile(
   					pattern,               /* the pattern */
   					0,                     /* default options */
   					&error,                /* for error message */
   					&erroffset,            /* for error offset */
   					NULL);                 /* use default character tables */
			if( robot.ptr == NULL )
			{
				return -4;
			}
		
		break;
		case STR_DISALLOW_REGEX:
			//printf("Reg Disallow: %s\n",robostr);
			robot.type = ROBOTS_DISALLOW_REGEX;

			if (transform_pattern( robostr+1 , len - 1 , pattern , MAX_ROBOTS_PATTERN_LENGTH ) != 0)
			{
				return -5;
			}
			//printf("Reg Disallow: %s\n", pattern);

			robot.ptr = (unsigned char *)pcre_compile(
                                        pattern,               /* the pattern */
                                        0,                     /* default options */
                                        &error,                /* for error message */
                                        &erroffset,            /* for error offset */
                                        NULL);                 /* use default character tables */
 
			if( robot.ptr == NULL )
			{
				return -5;
			}
		break;
		default:
			return -3;
	}


	return 0;
}

int 
clear_robot( robot_t robot )
{
	free(robot.ptr);
	robot.ptr = NULL;
	return 0;
}

#define OVECCOUNT 30     /* should be a multiple of 3 */ 

int 
check_match( char *path , int path_len,  robot_t robot )
{
	
	int ovector[OVECCOUNT];

	int ret = 0;


	switch(robot.type)
	{
		case ROBOTS_ALLOW_TEXT:
		case ROBOTS_DISALLOW_TEXT:
		case ROBOTS_ALLOW_COMMON:
		case ROBOTS_DISALLOW_COMMON:
			if( 0 == strncmp( path , (char*)robot.ptr , strlen((char*)robot.ptr)))
			{
				return ROBOTS_MATCH;
			}
			else
			{
				return ROBOTS_NOMATCH;
			}
		//break;
		case ROBOTS_ALLOW_REGEX:
		case ROBOTS_DISALLOW_REGEX:
			ret = pcre_exec(
			   (pcre *)robot.ptr,             /* the compiled pattern */
			   NULL,                  /* no extra data - we didn't study the pattern */
			   path,               /* the subject string */
			   (int)strlen(path),        /* the length of the subject */
			   0,                     /* start at offset 0 in the subject */
			   0,                     /* default options */
			   ovector,               /* output vector for substring information */
			   OVECCOUNT);            /* number of elements in the output vector */

			if( ret >= 0) 
			{
				return ROBOTS_MATCH;
			}
			else if (ret == PCRE_ERROR_NOMATCH )
			{
				return ROBOTS_NOMATCH;
			}
			else
			{
				return ROBOTS_ERROR;
			}
		//break;
		default:
			return ROBOTS_ERROR;	
	}
	//return ROBOTS_NOMATCH;
}


