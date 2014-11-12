#ifndef SPIDNS_ROBOTS_H
#define SPIDNS_ROBOTS_H

//#include <regex.h>

#define	ROBOTS_INVALID		0
#define	ROBOTS_ALLOW_COMMON	1
#define	ROBOTS_DISALLOW_COMMON	2
#define	ROBOTS_ALLOW_TEXT	3
#define	ROBOTS_DISALLOW_TEXT	4
#define	ROBOTS_ALLOW_REGEX	5
#define	ROBOTS_DISALLOW_REGEX	6

#define STR_ALLOW_COMMON	'+'
#define STR_DISALLOW_COMMON	'-'
#define STR_ALLOW_TEXT		'A'
#define STR_DISALLOW_TEXT	'D'
#define STR_ALLOW_REGEX		'*'
#define STR_DISALLOW_REGEX	'!'


typedef struct _robot_item
{
	int type;
	unsigned char *ptr;	
}robot_t;


int parse_robot( char *robostr , int len , robot_t &robot );
int clear_robot( robot_t robot );


#define ROBOTS_ERROR		-1
#define ROBOTS_NOMATCH		0
#define ROBOTS_MATCH		1

int check_match( char *path , int path_len,  robot_t robot );


#endif
