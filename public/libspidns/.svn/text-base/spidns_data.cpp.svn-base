/***************************************************************************
 * 
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: spidns_data.cpp,v 1.3 2008/09/25 07:33:46 jiangjinpeng Exp $ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file spidns_data.cpp
 * @author chentj(spider@baidu.com)
 * @date 2008/03/05 11:40:20
 * @version $Revision: 1.3 $ 
 * @brief 
 *  
 **/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <zlib.h>

#include "ul_sign.h"
#include "ul_log.h"

#include "spidns_data.h"
#include "spidns.h"

int parse_dns_robots_data(char *robots, siteinfo_t &output_site); 
/**
 * @brief 
 *
 * @param [in/out] cache_size   : int
 * @return  dns_data_t* 
 * @retval   
 * @see 
 * @note 
 * @author chentj
 * @date 2008/03/05 11:53:55
**/
dns_data_t *
init_dnsdata( int cache_size  )
{
	dns_data_t *pret = new dns_data_t;
	if ( pret == NULL )
	{	
		ul_writelog( UL_LOG_FATAL , "Cannot init dns data pool" );
		return NULL;
	}	

	pret->sn_len = 0;
	pret->sn_num = 0;

	return pret;
}


int  init_dnsmode( int mode, dns_mode_t * dns_mode )
{
	int	ip_mode = 0;
	
	assert( dns_mode != NULL );

	//初始化ip模式
	ip_mode = mode & 0x000f;
	switch(ip_mode)
	{
		case IP_SINGLE:
		case IP_MULTI:
			dns_mode->ip = ip_mode;
			break;
		default:
			ul_writelog(UL_LOG_FATAL, "init dns mode failed: ip mode[%d] , mode[%d]", ip_mode, mode);
			return -1;
	} 
	
	return 0;
}

void init_siteinfo( siteinfo_t& siteinfo)
{
	siteinfo.robots_data = NULL;
	siteinfo.robots_data_num = 0;
	siteinfo.ip_num = 0;
	siteinfo.ip = NULL;
	siteinfo.ipregion[0] = '\0';
	siteinfo.casesense[0] = '\0';	
	siteinfo.ishttp11 = 1;
}

void clear_siteinfo(siteinfo_t& siteinfo)
{
	for( int i = 0 ; i < siteinfo.robots_data_num ; i++ )
	{
		 clear_robot(siteinfo.robots_data[i]);
	}

	if( siteinfo.robots_data != NULL )
	{
		delete [] siteinfo.robots_data;
		siteinfo.robots_data = NULL;	
	}
	
	siteinfo.robots_data_num = 0;

	if ( siteinfo.ip != NULL )
	{
		delete [] siteinfo.ip;
		siteinfo.ip = NULL;
	}

	siteinfo.ip_num = 0;

	siteinfo.ipregion[0] = '\0';
	siteinfo.casesense[0] = '\0';
	siteinfo.ishttp11 = 1;
}



/**
 * @brief 
 *
 * @param [in/out] pdns_data   : dns_data_t*
 * @return  void 
 * @retval   
 * @see 
 * @note 
 * @author chentj
 * @date 2008/03/26 22:38:36
**/
void 
release_dnsdata( dns_data_t* pdns_data )
{
	if( pdns_data == NULL )
		return ;
	
	clear_cache( pdns_data );
	clear_buffer( pdns_data );
	

	delete pdns_data;


}

/**
 * @brief 
 *
 * @param [in/out] sitename   : char*
 * @param [in/out] len   : int
 * @return  unsigned int 
 * @retval   
 * @see 
 * @note 
 * @author chentj
 * @date 2008/03/05 11:55:43
**/
unsigned int 
get_sitesign(const char *sitename , int len )
{
	unsigned int sign_1 = 0;
	unsigned int sign_2 = 0;

	creat_sign_md64(const_cast<char*>(sitename), len, &sign_1, &sign_2 );

	return sign_1+sign_2;
}

/**
 * @brief 
 *
 * @param [in/out] pdns_data   : dns_data_t*
 * @param [in/out] sitename   : char*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author chentj
 * @date 2008/03/05 11:55:48
**/
int 
add_buffer( dns_data_t* pdns_data , const char *sitename , int site_len)
{
	char *ptr;
	unsigned char temp[ DNS_MAX_PACK_NUM * DNS_MAX_SITE_LEN ];
	//unsigned long templen = DNS_MAX_PACK_NUM * DNS_MAX_SITE_LEN;
	//char temp2[ DNS_MAX_PACK_NUM * DNS_MAX_SITE_LEN ];
	snbuf_t tmp_sn ;
	
	int ret = 0;
	unsigned long len = DNS_MAX_PACK_NUM * DNS_MAX_SITE_LEN;

	if ( pdns_data->sn_num >= DNS_MAX_PACK_NUM )	
	{
		
		ret = compress( (Bytef*)temp , (uLongf*)&len ,(const Bytef*) pdns_data->sn_buf , (size_t)pdns_data->sn_len );
		if( ret != Z_OK )
		{
			fprintf(stderr,"Cannot compress buffer");
			return -1;
		}
/*
///DEBUG USE
		ret = uncompress( (Bytef*)temp2 , (uLongf*)&templen , ( const Bytef*) temp , len );
		if (ret != Z_OK )
		{
			fprintf(stderr,"Cannot uncompress buffer");
			return -1;
		}
*/
//		printf("\n%ld\n%s\n" ,templen, temp2);

		tmp_sn.buf = new unsigned char[len+1];
		if( tmp_sn.buf == NULL )
		{
			return -1;
		}
		memcpy((void*)tmp_sn.buf , (void* )temp , len );
		tmp_sn.buf[len] = 0;
		tmp_sn.len = (int)len ;
		tmp_sn.num = pdns_data->sn_num ;
		pdns_data->sitename_data.push_back(tmp_sn);

//		printf("push buffer %d : %d\n",tmp_sn.len , tmp_sn.num);
/*
		for( int i = 0;i<=tmp_sn.len ;i++)
		{
			printf(" %x:%x ",temp[i],tmp_sn.buf[i]);

		}
*/
		pdns_data->sn_num = 0;
		pdns_data->sn_len = 0;	
	}
	
	ptr =  pdns_data->sn_buf + pdns_data->sn_len;	

	memcpy( (void*)ptr , (void*)sitename , (size_t)site_len );
	strcpy( ptr+site_len ,"\r\n" );

	pdns_data->sn_num++;
	pdns_data->sn_len += (site_len+2);
	
	
	return 0;
}

/**
 * @brief 
 *
 * @param [in] pdns_data   : dns_data_t*
 * @param [in] sitename   : char*
 * @param [in] siteinfo_addr   : int
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author chentj
 * @date 2008/03/05 11:55:54
**/
int 
update_siteinfo_cache( dns_data_t* pdns_data , unsigned int site_key , siteinfo_t  siteinfo )
{

	clear_siteinfo( pdns_data->res_index[site_key] );
	pdns_data->res_index[ site_key ] = siteinfo;
	
	return 0;
}


int
add_siteinfo_cache( dns_data_t* pdns_data , unsigned int site_key , siteinfo_t  siteinfo )
{
    pair< map<unsigned int , siteinfo_t>::iterator , bool > insert_res;

	insert_res = 
		pdns_data->res_index.insert( pair<unsigned int , siteinfo_t>( site_key, siteinfo ));

	if( true == insert_res.second )
	{
		return 0;
	}
	else
	{
		return -1;
	}

}



/**
 * @brief 
 *
 * @param [in/out] pdns_data   : dns_data_t*
 * @param [in/out] site_key   : unsigned int
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author chentj
 * @date 2008/03/05 11:55:59
**/
int 
seek_siteinfo( dns_data_t* pdns_data , unsigned int site_key , siteinfo_t &output_info  )
{
	unsigned int s_sign = site_key;
	map<unsigned int , siteinfo_t>::iterator iter;
    
	iter = pdns_data->res_index.find(s_sign);
	
	if(iter != pdns_data->res_index.end())
	{
		output_info = iter->second;
		return 0;
	}
	else
	{
	
		return -1;
	}
}

/**
 * @brief 
 *
 * @param [in/out] pdns_data   : dns_data_t*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author chentj
 * @date 2008/03/05 11:56:05
**/
int 
clear_cache( dns_data_t* pdns_data )
{
	
	map<unsigned int, siteinfo_t>::iterator iter;

    for(iter = pdns_data->res_index.begin(); iter != pdns_data->res_index.end(); iter++)
    {
        clear_siteinfo(iter->second);
    }

	pdns_data->res_index.clear();
	return 0;
}

/**
 * @brief 
 *
 * @param [in/out] pdns_data   : dns_data_t*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author chentj
 * @date 2008/03/05 11:56:10
**/
int 
clear_buffer( dns_data_t* pdns_data )
{
	//clear temp buffer	

	pdns_data->sn_buf[0] = '\0';
	pdns_data->sn_len = 0;
	pdns_data->sn_num = 0;

	//clear unresolve buffer
	vector<snbuf_t>::iterator iter;

	for( iter = pdns_data->sitename_data.begin(); iter != pdns_data->sitename_data.end(); iter++ )
	{
		delete []((snbuf_t*)iter->buf );
	}	

	pdns_data->sitename_data.clear();


	return 0;
}

int 
pop_buffer( dns_data_t* pdns_data , char *buf , int *buflen )
{
	int ret = 0;
	unsigned char temp[ DNS_MAX_PACK_NUM * DNS_MAX_SITE_LEN ];
	long templen = DNS_MAX_PACK_NUM * DNS_MAX_SITE_LEN;
	snbuf_t temp_sn;

	if ( pdns_data == NULL || buf == NULL )
	{
		return -1;
	}

	
	if ( pdns_data->sn_num > 0 )
	{

		//printf("Pop A Plan Text Buffer! %d : %d\n", pdns_data->sn_len, pdns_data->sn_num);

		if ( *buflen < pdns_data->sn_len )
		{
			return -1;
		}

		ret = pdns_data->sn_num ;
		pdns_data->sn_num = 0;
		*buflen = pdns_data->sn_len ;
		pdns_data->sn_len = 0;

		memcpy( buf , pdns_data->sn_buf , (size_t)*buflen );
		buf[*buflen] = '\0';

		return ret;
	}
	else if ( pdns_data->sitename_data.size() > 0 )
	{


//		temp_sn = pdns_data->sitename_data.pop_back();

		temp_sn = pdns_data->sitename_data[0];

		/*
        printf("Pop A Buffer from Vector! %d : %d\n", temp_sn.len, temp_sn.num );

		for( int i = 0 ; i <= temp_sn.len;i++ )
		{
			printf(" %x",temp_sn.buf[i] );
		}

		fflush(stdout);
*/

		ret = uncompress( (Bytef*)temp , (uLongf*)&templen ,(const Bytef*) temp_sn.buf , (size_t)temp_sn.len );
        if( ret != Z_OK )
        {
			fprintf( stderr, "Uncompress Buffer from Vector Error!(%d:%ld)\n",ret,templen);
            return -1;
        }
		else if ( templen > *buflen )
		{
			fprintf( stderr, "Uncompress Buffer Length Error!(%d:%ld)\n",*buflen , templen);
			return -1;
		}
		else
		{
			memcpy( buf , temp , (size_t)templen);
			*buflen = templen ;
			ret = temp_sn.num ;
			
			delete []( temp_sn.buf );
			pdns_data->sitename_data.erase( pdns_data->sitename_data.begin() );			

			return ret;
		}

	}
	else
	{
		return 0;
	}
}



int get_item(char **start, char *end , char* value, int len , char d , const char*name)
{
	char *p = *start;
	char *q = NULL;
	int i = 0;

	q = strchr(p, d);

	if (q == NULL || q > end)
    {
            return -1;
    }

    if (p == q || (q - p + 1) > len)
    {
//          ul_writelog(UL_LOG_NOTICE, "parse_dnsline: %s is empty or too long:%s",name, p);
            //this value is used in parse_dumpline
			value[0] = 0;
			return -1;
    }
    
	i = 0;
    while (p != q)
    {
            value[i] = *p;
            i++;
            p++;
    }
    value[i] = '\0';

    *start = q + 1;
	return 0;
}


int
parse_dnsdumpline(char *linestart , char *lineend , char *sitename , int len , siteinfo_t &output_site )
{
/*
    Format Of dns Readable Dump file
	
	"%s %s %u %c %u %u %u %u %u %s\n"
	Colum:
    1.sitename                                      
	2.siteip
    3.state                       
	4.ipregion
    5.http_version      
	6.site_level
	7.case_sensitive
	8.robots_count
    9.robots_length       
   10.robotsdata

*/	


	char *p = linestart;
	char ipstr[32];
	char status[32];
	char sitelevel[32];
	char httpver[32];
	char robotsflag[32];
	char robots[4096];

	in_addr addr_in;

	sitename[0] = '\0';
	ipstr[0] = '\0';

	//Get Site Name
	if ( 0 > get_item( &p , lineend , sitename , len , ' ' , "sitename"))
	{
		return -1;
	}
	
	//Get IP String
	if ( 0 > get_item( &p , lineend , ipstr , 32 , ' ' , "IP"))
	{
		return -1;
	}

	int n = inet_pton (AF_INET, ipstr, &addr_in);
	output_site.ip = new unsigned int[1];
	if( output_site.ip == NULL )
	{
		ul_writelog(UL_LOG_WARNING,"[%s]: Cannot malloc ip memory", __FUNCTION__);
		return -1;
	}
	

	if (n <= 0)
	{
		output_site.ip[0] = 0;
	}
	else
	{
		output_site.ip[0] = addr_in.s_addr;
	}
	output_site.ip_num = 1;

    //Get Site Status
    if ( 0 > get_item( &p , lineend , status , 32 , ' ' , "Site Status"))
    {
        return -1;
    }


	//Get IP Region
	if ( 0 > get_item( &p , lineend , output_site.ipregion , 4 , ' ' , "IP region" ))
	{
		return -1;
	}

    //Get Http Version
    if ( 0 > get_item( &p , lineend , httpver , 32 , ' ' , "HTTP Version"))
    {
        return -1;
    }
    //Get Site Level
    if ( 0 > get_item( &p , lineend , sitelevel , 32 , ' ' , "Site Level"))
    {
        return -1;
    }


	//Get CaseSensitive
	if ( 0 > get_item( &p , lineend , output_site.casesense , 4 , ' ' , "Case Sensitive"))
	{
		return -1;
	}

	//Get Robots Flag
	if ( 0 > get_item( &p , lineend , robotsflag , 32 , ' ' , "Robots Flag"))
	{
		return -1;
	}

    if ( 0 > get_item( &p , lineend , robotsflag , 32 , ' ' , "Robots Flag"))
    {
        return -1;
    }

	//May Cause Error here
	get_item( &p , lineend , robots , 4096 , '\n' , "Robots String");

	if( 0 > parse_dns_robots_data( robots, output_site))
	{
		return -1;
	}	

	
	return 0;



}

/**
 * @brief 
 *
 * @param [in/out] linebuf   : char*
 * @param [in/out] len   : int
 * @param [in/out] output_site   : siteinfo_t&
 * @see 
 * @note 
 * @author chentj
 * @date 2008/04/09 21:43:30
**/
int
parse_resultline_singleip( char *linestart , char *lineend , char *sitename , int len , siteinfo_t &output_site )
{
	char *p = linestart;
	char ipstr[32];
    char robotsflag[32];
    char robots[4096];
	in_addr addr_in;

	sitename[0] = '\0';
	ipstr[0] = '\0';

	//Get Site Name
	if ( 0 > get_item( &p , lineend , sitename , len , ' ' , "sitename"))
	{
		return -1;
	}
	
	//Get IP String
	if ( 0 > get_item( &p , lineend , ipstr , 32 , ' ' , "IP"))
	{
		return -1;
	}

	int n = inet_pton (AF_INET, ipstr, &addr_in);

	output_site.ip = new unsigned int[1];
	if( output_site.ip == NULL )
	{
		ul_writelog(UL_LOG_WARNING, "[%s]:Cannot malloc ip memory",__FUNCTION__);
		return -1;
		
	}
	if (n <= 0)
	{
		output_site.ip[0] = 0;
	}
	else
	{
		output_site.ip[0] = addr_in.s_addr;
	}
	output_site.ip_num = 1;

    
	//Get IP Region
	if ( 0 > get_item( &p , lineend , output_site.ipregion , 4 , ' ' , "IP region" ))
	{
		return -1;
	}

	//Get CaseSensitive
	if ( 0 > get_item( &p , lineend , output_site.casesense , 4 , ' ' , "Case Sensitive"))
	{
		return -1;
	}

    //Get Robots Flag

    if ( 0 > get_item( &p , lineend , robotsflag , 32 , ' ' , "Robots Flag"))
    {
        return -1;
    }

    //Get Robots String
    if ( 0 > get_item( &p , lineend , robots , 4096 , '\r' , "Robots String"))
    {
        return -1;
    }

	if ( 0 > parse_dns_robots_data(robots, output_site) )
	{
		return -1;
	}
	
	return 0;
}


int
parse_resultline_multiip( char *linestart , char *lineend , char *sitename , int len , siteinfo_t &output_site )
{
    char *p = linestart;
    char ipstr[32];
	char ipcount[32];
    char robotsflag[32];
    char robots[4096];
	int	i = 0;
    in_addr addr_in;

    sitename[0] = '\0';
    ipstr[0] = '\0';
	

    //Get Site Name
    if ( 0 > get_item( &p , lineend , sitename , len , ' ' , "sitename"))
    {
        return -1;
    }


    //Get IP Region
    if ( 0 > get_item( &p , lineend , output_site.ipregion , 4 , ' ' , "IP region" ))
    {
        return -1;
    }

    //Get CaseSensitive
    if ( 0 > get_item( &p , lineend , output_site.casesense , 4 , ' ' , "Case Sensitive"))
    {
        return -1;
    }

	//Get Ip count
	if ( 0 > get_item( &p , lineend , ipcount , 32 , ' ' , "Ip Count"))
    {
        return -1;
    }
	output_site.ip_num=strtol(ipcount, NULL, 10);
	if(output_site.ip_num <= 0 )
	{
		output_site.ip_num = 0;
		ul_writelog(UL_LOG_WARNING,"[%s]: site [%s] ip count [%d] is invalid",
					__FUNCTION__, sitename, output_site.ip_num);
		return -1;
	}
	
	//Get Ip
	output_site.ip = new unsigned int[output_site.ip_num];
	if(output_site.ip == NULL)
	{
		ul_writelog(UL_LOG_WARNING,"[%s]: Cannot malloc ip memory",__FUNCTION__);
		return -1;
	}
	for( i = 0; i < output_site.ip_num; i++)
	{
		if ( 0 > get_item( &p , lineend , ipstr , 32 , ' ' , "IP"))
		{
			return -1;
		}

		int n = inet_pton (AF_INET, ipstr, &addr_in);

		if (n <= 0)
		{
			output_site.ip[i] = 0;
		}
		else
		{
			output_site.ip[i] = addr_in.s_addr;
		}
	}
    
    
	//Get Robots Flag
    
    if ( 0 > get_item( &p , lineend , robotsflag , 32 , ' ' , "Robots Flag"))
    {   
        return -1;
    }
    
    //Get Robots String
    if ( 0 > get_item( &p , lineend , robots , 4096 , '\r' , "Robots String"))
    {   
        return -1;
    }

    if ( 0 > parse_dns_robots_data(robots, output_site) )
    {
        return -1;
    }

    return 0;
}

int parse_dns_robots_data(char *robots, siteinfo_t &output_site)
{
	char robotstr[4096];
	robot_t temp_robot;
	vector<robot_t> robot_v;
	char *end = NULL;
	int v_size = 0;
	int tmplen = 0;
	char * p =NULL;

////DEBUG USE
//	robots[0]=0;
	

	if(strcmp(robots, "@NULL@")==0)
	{
		robots[0] = '\0';
		output_site.robots_data = NULL;
		output_site.robots_data_num = 0;
	}
	else
	{
		//parse Robots String
		end = robots+ strlen( robots);
		p = robots;
		while( 0 == get_item( &p , end , robotstr , 4096 , ' ' , "Robots Item"))
		{
			if ( 0 == parse_robot( robotstr , (int)strlen( robotstr ) , temp_robot ))
			{
				robot_v.push_back( temp_robot );
			}
		}
		
		if ( p < end )
		{
			tmplen = end - p + 1;
			memcpy(robotstr, p, (size_t)tmplen);
			robotstr[ tmplen ] = 0;
			if ( 0 == parse_robot( robotstr , (int)strlen( robotstr) , temp_robot))
			{
				robot_v.push_back( temp_robot );
			}
		}
		
		v_size = (int)robot_v.size();		

		if ( v_size > 0 )
		{
			output_site.robots_data = new robot_t[v_size];

			if ( output_site.robots_data == NULL )
			{
				fprintf(stderr,"cannot alloc robots\n");
				output_site.robots_data_num = 0;
				return -1;
			}

			for( int i = 0 ; i < v_size ; i++)
			{
				memcpy(&(output_site.robots_data[i]), &robot_v[i], sizeof(robot_t));
			}	
			output_site.robots_data_num = v_size;
		}
		else
		{
			 output_site.robots_data = NULL;
			 output_site.robots_data_num = 0;
		}
		
	}
	

	
	return 0;

}


/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
