#include <stdio.h>
#include <stdlib.h>

#include "common_func.h"

/*******************************************************************
 *description:
 *	To copy text from source to destination
 *input:
 *	char *src : pointer to source text
 *	int avail : the first available position of dest array.
 *	int size  : size of dest array
 *output :
 *	char *dest :
 *		pointer to dest text buffer
 *return value:
 *  avail: the first avail position in dest buffer
 *		after copy source text into dest buffer
 **********************************************************************/
int _copy_text(char *dest,int avail,int size,char *src)
{
#ifdef DEBUG
    assert(dest!=NULL);
    assert(src!=NULL);
#endif    
	char *p = src;
	while((*p!=0) && avail<size)
	{
		dest[avail++] = *p ++;
	}
	if(avail!= size)
		dest[avail] = 0;
	else
		dest[avail-1] = 0;
	return avail;	
}

/*******************************************************************
 *description:
 *	To get a catch_js_cmd packet
 *input:
 *	js_url_t *jsurl
 *		the javascript file's urls that we want js_catch_worker module to catch.
 *	int url_num;
 *		the url's num in jsurl array.	
 *output :
 *	char *pack :
 *		 the catch_js_cmd pack
 *return value:
 *		the total packet length including head and body.
 **********************************************************************/
int make_catch_js_pack(char *pack,int pack_len,js_url_t *jsurl,int *url_num)
{
#ifdef DEBUG
    assert(pack!=NULL);
    assert(jsurl!=NULL);
#endif    
	if(pack_len<PACK_BODY_BYTE)	
	{
		ul_writelog(UL_LOG_NOTICE,"pack len is illegal.[Pack_len:%d]\r\n",pack_len);
		return FAIL;
	}	
	if(*url_num>MAX_URL_NUM)	
	{
		ul_writelog(UL_LOG_NOTICE,"too many js url number.[js_url_num:%d]\r\n",*url_num);
		*url_num = MAX_URL_NUM;
	}
	
	char *temp = &pack[PACK_BODY_BYTE];
	memset(pack,0,pack_len);
	pack[TYPE_BYTE] = CATCH_JS_CMD;
	pack[VER_BYTE] = VER;

	int _ret = 0;
	int left_size = pack_len - PACK_BODY_BYTE;
    int i;
	for(i =0; i<*url_num;i++)
	{
        if(_ret == left_size)   break;
		 _ret = _copy_text(temp,_ret,left_size,jsurl[i].url);
		 _ret = _copy_text(temp,_ret,left_size,"\r");
	}
    *url_num = i;
	int body_len = _ret;

	pack[LINK_NUM_BYTE]   =	(*url_num)/256;
	pack[LINK_NUM_BYTE+1] =	(*url_num)%256;
	
	pack[PACK_LEN_BYTE] = (body_len >> 24) & 0x000000ff;
	pack[PACK_LEN_BYTE+1] = (body_len >> 16) & 0x000000ff;
	pack[PACK_LEN_BYTE+2] = (body_len >> 8) & 0x000000ff;
	pack[PACK_LEN_BYTE+3] = (body_len ) & 0x000000ff;
	return PACK_BODY_BYTE+body_len;
}

/*******************************************************************
 *description:
 *	To extract all cmd head 
 *input:
 *	char *head
 *		a packet head	
 *output :
 *	int* url_num;
 *		the url's num extracted from catch_js_cmd head or run_jslink head.	
 *	int *packet_length :
 *		the pack body length extracted from command head	
 *	int *ack:
 *		the ack value extracted from command head	
 *return value:
 *		the command type(CATCH_JS_CMD,ACK_CATCH_CMD,GET_JS_CMD etc) or FAIL
 **********************************************************************/
int extract_cmd_head(char *head,int *url_num,int *packet_length,int *ack)
{
#ifdef DEBUG
    assert(head!=NULL);
#endif    
	int cmd;
	char *pack = head;
    if(packet_length!=NULL)
	    *packet_length = 0;
    if(url_num!=NULL)
	    *url_num = 0;
	if(pack[VER_BYTE] != VER)	return FAIL;
	cmd = pack[TYPE_BYTE];
	int ret = cmd;
	switch(cmd){
	case CATCH_JS_CMD:
        assert(packet_length!=NULL);
        assert(url_num!=NULL);
		for(unsigned i=0; i<LINK_NUM_BYTE_NUM;i++)
		{
			*url_num = ((*url_num)<<8) + (pack[LINK_NUM_BYTE+i]&0xff);
		}
		for(unsigned i=0; i<PACK_LEN_BYTE_NUM;i++)
		{
			
			*packet_length = ((*packet_length)<<8) +(pack[PACK_LEN_BYTE +i]&0xff);
		}
		if(*url_num<0||*url_num>MAX_URL_NUM)
			ul_writelog(UL_LOG_NOTICE," url num is illegal.[Cmd:%d] [URL Num:%d]\r\n",
			cmd,*url_num);
		if(*packet_length<0)
			ul_writelog(UL_LOG_NOTICE," packet length is illegal.[Cmd:%d] [Packet Length:%d]\r\n",
			cmd,*packet_length);
		break;
	
	case ACK_CATCH_CMD:
        assert(ack!=NULL);
        assert(packet_length!=NULL);
		*ack = pack[ACK_BYTE];
		for(unsigned i=0; i<PACK_LEN_BYTE_NUM;i++)
		{
			*packet_length = ((*packet_length)<<8) +(pack[PACK_LEN_BYTE]&0xff);
		}
		if(*packet_length<0)
			ul_writelog(UL_LOG_NOTICE," packet length is illegal.[Cmd:%d] [Packet Length:%d]\r\n",
			cmd,*packet_length);
		break;
	
	case GET_JS_CMD:
        assert(packet_length!=NULL);
		for(unsigned i=0; i<PACK_LEN_BYTE_NUM;i++)
		{
			*packet_length = ((*packet_length)<<8) +(pack[PACK_LEN_BYTE +i]&0xff);
		}
		if(*packet_length >= UL_MAX_URL_LEN||*packet_length<0)
			ul_writelog(UL_LOG_NOTICE," packet length is illegal.[Cmd:%d] [Packet Length:%d]\r\n",
			cmd,*packet_length);
		break;
		
	case RET_JS_CMD:
        assert(ack!=NULL);
        assert(packet_length!=NULL);
		*ack = pack[ACK_BYTE];
		for(unsigned i=0; i<PACK_LEN_BYTE_NUM;i++)
		{
			*packet_length = ((*packet_length)<<8) +(pack[PACK_LEN_BYTE +i]&0xff);
		}
		if(*packet_length<0)
			ul_writelog(UL_LOG_NOTICE," packet length is illegal.[Cmd:%d] [Packet Length:%d]\r\n",
			cmd,*packet_length);
		break;
		
	case RUN_JSLINK_CMD:
        assert(url_num!=NULL);
        assert(packet_length!=NULL);
		for(unsigned i=0; i<LINK_NUM_BYTE_NUM;i++)
		{
			*url_num = ((*url_num)<<8) + (pack[DYNLINK_NUM_BYTE+i]&0xff);
		}
		for(unsigned i=0; i<PACK_LEN_BYTE_NUM;i++)
		{
			*packet_length = ((*packet_length)<<8) +(pack[PACK_LEN_BYTE +i]&0xff);
		}
		if(*url_num<0||*url_num>MAX_URL_NUM)
			ul_writelog(UL_LOG_NOTICE," url num is illegal.[Cmd:%d] [URL Num:%d]\r\n",
			cmd,*url_num);
		if(*packet_length<0)
			ul_writelog(UL_LOG_NOTICE," packet length is illegal.[Cmd:%d] [Packet Length:%d]\r\n",
			cmd,*packet_length);
		break;

	case ACK_RUN_CMD:
        assert(ack!=NULL);
        assert(packet_length!=NULL);
		*ack = pack[ACK_BYTE];
		for(unsigned i=0; i<PACK_LEN_BYTE_NUM;i++)
		{
			*packet_length = ((*packet_length)<<8) +(pack[PACK_LEN_BYTE +i]&0xff);
		}
		if(*packet_length<0)
			ul_writelog(UL_LOG_NOTICE," packet length is illegal.[Cmd:%d] [Packet Length:%d]\r\n",
			cmd,*packet_length);
		break;
		
	case RET_LINK_CMD:
        assert(ack!=NULL);
        assert(packet_length!=NULL);
		*ack = pack[ACK_BYTE];
		for(unsigned i=0; i<PACK_LEN_BYTE_NUM;i++)
		{
			*packet_length = ((*packet_length)<<8) +(pack[PACK_LEN_BYTE +i]&0xff);
		}
		if(*packet_length<0)
			ul_writelog(UL_LOG_NOTICE," packet length is illegal.[Cmd:%d] [Packet Length:%d]\r\n",
			cmd,*packet_length);
		break;
    
    case RUN_WRITED_PAGE_JSLINK_CMD:
        assert(url_num!=NULL);
        assert(packet_length!=NULL);
		for(unsigned i=0; i<LINK_NUM_BYTE_NUM;i++)
		{
			*url_num = ((*url_num)<<8) + (pack[DYNLINK_NUM_BYTE+i]&0xff);
		}
		for(unsigned i=0; i<PACK_LEN_BYTE_NUM;i++)
		{
			*packet_length = ((*packet_length)<<8) +(pack[PACK_LEN_BYTE +i]&0xff);
		}
		if(*url_num<0||*url_num>MAX_URL_NUM)
			ul_writelog(UL_LOG_NOTICE," url num is illegal.[Cmd:%d] [URL Num:%d]\r\n",
			cmd,*url_num);
		if(*packet_length<0)
			ul_writelog(UL_LOG_NOTICE," packet length is illegal.[Cmd:%d] [Packet Length:%d]\r\n",
			cmd,*packet_length);
		break;
    
    case RET_WRITED_PAGE_CMD:    
        assert(ack!=NULL);
        *ack = pack[ACK_BYTE];
		for(unsigned i=0; i<PACK_LEN_BYTE_NUM;i++)
		{
			*packet_length = ((*packet_length)<<8) +(pack[PACK_LEN_BYTE +i]&0xff);
		}
		if(*packet_length<0)
			ul_writelog(UL_LOG_NOTICE," packet length is illegal.[Cmd:%d] [Packet Length:%d]\r\n",
			cmd,*packet_length);
		break;

	default:
		ret = FAIL;
		break;
	}
	return ret;
}

/*******************************************************************
 *description:
 *	To get an url list to be catched from packet body of the js_catch_cmd pack
 *input:
 *	char *body:
 *		packet body of the js_catch_cmd pack
 *	int packet_length:
 *		the size of body array.
 *	int url_num;
 *		the url num included in the body 	
 *output :
 *	js_url_t *js_url :
 *		 the ack_catch_cmd pack
 *return value:
 *		 url num extracted from pack body
 **********************************************************************/
int extract_js_catch_body(char *body,js_url_t *js_url,int url_num,int packet_length)
{
   assert(body!=NULL);
   assert(js_url!=NULL);
   
   body[packet_length] = 0;
	
	char *url_start = body;
	int i;
	for(i=0;i<url_num;i++)
	{
		char *url_end = strchr(url_start,'\r');
		if(url_end!=NULL)
		{
			*url_end = 0;
			if((url_end -url_start) >= UL_MAX_URL_LEN)
			{
				ul_writelog(UL_LOG_NOTICE,"url len is too long.[url len:%zu]\r\n",url_end-url_start);
				memcpy(js_url[i].url,url_start,UL_MAX_URL_LEN);
			}else{
				memcpy(js_url[i].url,url_start,	url_end-url_start+1);
			}
			url_start = url_end + 1;
			js_url[i].url[UL_MAX_URL_LEN-1] = 0;
			
		}else{
			if((packet_length-(url_start -body)) >= UL_MAX_URL_LEN)
			{
				memcpy(js_url[i].url,url_start,UL_MAX_URL_LEN);
			}else{
				strncpy(js_url[i].url,url_start,	packet_length-(url_start-body)+1);
			}
			js_url[i].url[UL_MAX_URL_LEN-1] = 0;
			break;
		}	
	}
	return i;
}

/*******************************************************************
 *description:
 *	To get a ack_catch_cmd packet
 *input:
 *	int ack:
 *		the ACK value to be returned to the js_center
 *	int pack_len;
 *		value = 0.	
 *output :
 *	char *pack :
 *		 the ack_catch_cmd pack
 *return value:
 *		 total size of the pack including the body and head
 **********************************************************************/
int make_ack_catch_pack(char *pack,int pack_len,int ack)
{
    assert(pack!=NULL);
	if(pack_len<PACK_BODY_BYTE)	
	{
		ul_writelog(UL_LOG_NOTICE,"pack len is illegal.[Pack_len:%d]\r\n",pack_len);
		return FAIL;
	}	
	memset(pack,0,pack_len);
	pack[TYPE_BYTE] = ACK_CATCH_CMD;
	pack[VER_BYTE] = VER;
	pack[ACK_BYTE] = ack;
	return PACK_BODY_BYTE;
}

/*******************************************************************
 *description:
 *	To get a ack_catch_cmd packet
 *input:
 *	int pack_len:
 *		the size of pack buffer
 *	int http_status:
 *		http return value (2XX,3XX,4XX) and OK
 *output :
 *	char *pack :
 *		 the ret_js_cmd pack
 *return value:
 *		 total size of the pack including the body and head
 **********************************************************************/
int make_ret_js_pack(char *pack,int pack_len,int http_status,char *page,int page_len)
{
    assert(pack!=NULL);
    assert(page!=NULL);
	if(page_len<0||page_len>(pack_len-PACK_BODY_BYTE))
	{
		ul_writelog(UL_LOG_NOTICE,"page len is illegal.[Page_len:%d]\r\n",page_len);
		return FAIL;
	}	
	memset(pack,0,pack_len);
	pack[TYPE_BYTE] = RET_JS_CMD;
	pack[VER_BYTE] = VER;
	pack[ACK_BYTE] = http_status;
	pack[PACK_LEN_BYTE] = (page_len >> 24) & 0xff;
	pack[PACK_LEN_BYTE+1] = (page_len >> 16) & 0xff;
	pack[PACK_LEN_BYTE+2] = (page_len >> 8) & 0xff;
	pack[PACK_LEN_BYTE+3] = (page_len ) & 0xff;
	memcpy(&pack[PACK_BODY_BYTE],page,page_len);
	return page_len+PACK_BODY_BYTE;
}

/*******************************************************************
 *description:
 *	To get a get_js_pack packet
 *input:
 *	int pack_len:
 *		the size of pack buffer
 *	char *url:
 *		the url to be read from js-catch-worker
 *output :
 *	char *pack :
 *		 the get_js_pack pack
 *return value:
 *		 total size of the pack including the body and head
 **********************************************************************/
int make_get_js_pack(char *pack,int pack_len,char *url)
{
    assert(pack!=NULL);
    assert(url!=NULL);
	memset(pack,0,pack_len);
	pack[TYPE_BYTE] = GET_JS_CMD;
	pack[VER_BYTE] = VER;
	int len = strlen(url);
	if(len>(pack_len - PACK_BODY_BYTE))
	{
		ul_writelog(UL_LOG_NOTICE,"url len is too long.[url len:%d]\r\n",len);
		return FAIL;
	}
	pack[PACK_LEN_BYTE] = (len >> 24) & 0xff;
	pack[PACK_LEN_BYTE+1] = (len >> 16) & 0xff;
	pack[PACK_LEN_BYTE+2] = (len >> 8) & 0xff;
	pack[PACK_LEN_BYTE+3] = (len ) & 0xff;
	memcpy(&pack[PACK_BODY_BYTE],url,len);
	return len+PACK_BODY_BYTE;
}

/*******************************************************************
 *description:
 *	To get a get_js_pack packet
 *input:
 *	int pack_len:
 *		the size of pack buffer
 *	char *script:
 *		the script needed by js_run_script
 *	int script_len:
 *		size of script buffer
 *	dyn_link_t *dyn_link
 *		the links generated with javascript
 *	int dyn_link_num
 *		the num of dyn_link
 *output :
 *	char *pack :
 *		 the get_js_pack pack
 *return value:
 *		 total size of the pack including the body and head
 **********************************************************************/
int make_run_jslink_pack(char *pack,int pack_len,char *url,char *script,int script_len,dyn_link_t *dyn_link,int *dyn_link_num)
{
	if((unsigned)pack_len<PACK_BODY_BYTE+script_len+strlen(url))
	{
		ul_writelog(UL_LOG_NOTICE,"pack len is illegal.[Pack_len:%d]\r\n",pack_len);
		return FAIL;
	}	
	if(*dyn_link_num>MAX_URL_NUM)	
	{
		ul_writelog(UL_LOG_NOTICE,"too many dyn link number.[Dyn_link_num:%d]\r\n",*dyn_link_num);
		*dyn_link_num = MAX_URL_NUM;
	}
#ifdef DEBUG
    assert(dyn_link!=NULL);
    assert(script!=NULL);
    assert(url!=NULL);
    assert(pack!=NULL);
#endif

	memset(pack,0,pack_len);

	pack[TYPE_BYTE] = RUN_JSLINK_CMD;
	pack[VER_BYTE] = VER;
    
	
	int left_size = pack_len -PACK_BODY_BYTE;
	int _ret = 0;
	char *temp = &pack[SCRP_BYTE];

	_ret = _copy_text(temp,_ret,left_size,url);
    temp[_ret] = 0;
    _ret ++;
    if(script_len + _ret > left_size){
        ul_writelog(UL_LOG_NOTICE,"script len is too long.script_len:%d url_len:%zu\r\n",script_len,strlen(url));
        return FAIL;
    }
   
    memcpy(&temp[_ret],script,script_len);
    _ret += script_len;
	char buffer[UL_MAX_URL_LEN+10];
    int i;
	for(i =0; i<*dyn_link_num;i++)
	{
//		ul_writelog(UL_LOG_DEBUG,"copy dyn link:[%d]\r\n",i);
		if(left_size<=0)	break;
		snprintf(buffer,UL_MAX_URL_LEN+10,"%s\t%d\r",dyn_link[i].js_link,dyn_link[i].offset);
		_ret = _copy_text(temp,_ret,left_size,buffer);
	}
	int len = _ret+SCRP_BYTE-PACK_BODY_BYTE;
    *dyn_link_num = i;
	
	pack[DYNLINK_NUM_BYTE] = (*dyn_link_num) /256;
	pack[DYNLINK_NUM_BYTE+1] = (*dyn_link_num) %256;
	pack[PACK_LEN_BYTE] = (len >> 24) & 0xff;
	pack[PACK_LEN_BYTE+1] = (len >> 16) & 0xff;
	pack[PACK_LEN_BYTE+2] = (len >> 8) & 0xff;
	pack[PACK_LEN_BYTE+3] = (len ) & 0xff;

    script_len += strlen(url)+1;
	pack[SCRP_LEN_BYTE] = (script_len >> 24) & 0xff;
	pack[SCRP_LEN_BYTE+1] = (script_len >> 16) & 0xff;
	pack[SCRP_LEN_BYTE+2] = (script_len >> 8) & 0xff;
	pack[SCRP_LEN_BYTE+3] = (script_len ) & 0xff;
	
	pack[DYNLINK_LEN_BYTE] = (len-script_len-8 >> 24) & 0xff;
	pack[DYNLINK_LEN_BYTE+1] = (len-script_len -8>> 16) & 0xff;
	pack[DYNLINK_LEN_BYTE+2] = (len -script_len-8 >> 8) & 0xff;
	pack[DYNLINK_LEN_BYTE+3] = (len - script_len-8 ) & 0xff;
	return len+PACK_BODY_BYTE;
}

/*******************************************************************
 *description:
 *	To get a ack_run_jslink command packet
 *input
 *	int pack_len:
 *		the size of pack buffer
 *	int ack:
 *		the ack value returned to the js_center(OK/FAIL/BUSY)
 *output :
 *	char *pack :
 *		 the ack_run_jslink pack
 *return value:
 *		 total size of the pack including the body and head
 **********************************************************************/
int make_ack_run_pack(char *pack,int pack_len,int ack)
{
#ifdef DEBUG
    assert(pack!=NULL);
#endif    
	if(pack_len<PACK_BODY_BYTE)	
	{
		ul_writelog(UL_LOG_NOTICE,"pack len is illegal.[Pack_len:%d]\r\n",pack_len);
		return FAIL;
	}	
	memset(pack,0,pack_len);
	pack[TYPE_BYTE] = ACK_RUN_CMD;
	pack[VER_BYTE] = VER;
	pack[ACK_BYTE] = ack;
	return PACK_BODY_BYTE;
}

/*******************************************************************
 *description:
 *	To get a ret_jslink_pack packet
 *input:
 *	int pack_len:
 *		the size of pack buffer
 *	int ack:
 *		the ack value about the link.(OK/FAIL)
 *output :
 *	char *pack :
 *		 the ret_jslink pack
 *return value:
 *		 total size of the pack including the body and head
 **********************************************************************/
int make_ret_link_pack(char *pack,int pack_len,int ack,char *link,int offset)
{
#ifdef DEBUG
    assert(pack!=NULL);
    assert(link!=NULL);
#endif    
	if(pack_len<PACK_BODY_BYTE)	
	{
		ul_writelog(UL_LOG_NOTICE,"pack len is illegal.[Pack_len:%d]\r\n",pack_len);
		return FAIL;
	}	
	
	memset(pack,0,pack_len);
	pack[TYPE_BYTE] = RET_LINK_CMD;
	pack[VER_BYTE] = VER;
	pack[ACK_BYTE] = ack;
	char buffer[UL_MAX_URL_LEN+10];
	snprintf(buffer,UL_MAX_URL_LEN+10,"%s\t%d",link,offset);
	int _ret = _copy_text(&pack[PACK_BODY_BYTE],0,pack_len-PACK_BODY_BYTE,buffer);
    
	pack[PACK_LEN_BYTE] = (_ret >> 24) & 0xff;
	pack[PACK_LEN_BYTE+1] = (_ret >> 16) & 0xff;
	pack[PACK_LEN_BYTE+2] = (_ret >> 8) & 0xff;
	pack[PACK_LEN_BYTE+3] = (_ret ) & 0xff;
	return _ret+PACK_BODY_BYTE;
}

/*******************************************************************
 *description:
 *	To extract the run_jslink pack 
 *input:
 *	int packet_length:
 *		the size of body buffer
 *	char *body:
 *		the body of run_jslink pack
 *	int js_link_num:
 *		number of dynamic javascript link extracted from run_jslink head 
 *output :
 *	int *offset :
 *		offset extracted from dynamic link in the body
 *	char *js_link:
 *		dynamic javascript link included in the body
 *	char *script :
 *		 the script included in the body array
 *return value:
 *		 number of dynamic javascript link extracted from body array
 **********************************************************************/
int extract_run_jslink_body(char *body,char (*js_link)[UL_MAX_URL_LEN],int *offset,char *url,char *script,
int packet_length,int js_link_num)
{
#ifdef DEBUG
    assert(body!=NULL);
    assert(script!=NULL);
    assert(offset!=NULL);
#endif    
	int script_length = 0;
	for(unsigned i=0; i<SCRP_LEN_BYTE_NUM;i++)
	{
		script_length = ((script_length)<<8) + (body[SCRP_LEN_BYTE-PACK_BODY_BYTE +i]&0xff);
	}	
	
	int dyn_link_length=0;
	for(unsigned i=0; i<DYNLINK_LEN_BYTE_NUM;i++)
	{
		dyn_link_length = ((dyn_link_length)<<8) + (body[DYNLINK_LEN_BYTE -PACK_BODY_BYTE+i]&0xff);
	}	

	if(packet_length != (dyn_link_length + script_length+SCRP_LEN_BYTE_NUM + DYNLINK_NUM_BYTE))
	{
		ul_writelog(UL_LOG_NOTICE,"run jslink packet:length is not equal.[Packet_len:%d] [Script_len:%d] [Dyn_lin_len:%d]\r\n",packet_length,script_length,dyn_link_length);
		return FAIL;
	}
	char *temp = &body[SCRP_BYTE-PACK_BODY_BYTE];
    strncpy(url,temp,UL_MAX_URL_LEN);
    url[UL_MAX_URL_LEN-1] = 0;
	memcpy(script,&temp[strlen(url)+1],script_length-strlen(url)-1);

    script[script_length-strlen(url)-1] = 0;
	char *pstart = &body[SCRP_BYTE+script_length-PACK_BODY_BYTE];
	int i;
    if(js_link_num>MAX_DYNLINK_NUM)    js_link_num = MAX_DYNLINK_NUM;
    
	for(i=0;i<js_link_num;i++)
	{
    label_begin:
		char *pend = strchr(pstart,'\r');
		if(pend!= NULL)	
		{
			char *poffset = strchr(pstart,'\t');
            if(poffset!=NULL)
            {
			    *pend = 0;
			    *poffset = 0;
			    snprintf((char *)(js_link+i),UL_MAX_URL_LEN,"%s",pstart);
			    *(offset+i) = atoi(poffset+1);
			    pstart = pend + 1; 
            }else{
                pstart = pend +1;
                goto label_begin;
            }    
                
		}else{
			char *poffset = strchr(pstart,'\t');
            if(poffset!=NULL)
            {
			    *poffset = 0;
		    	snprintf((char *)(js_link+i),UL_MAX_URL_LEN,"%s",pstart);
			    *(offset+i) = atoi(poffset+1);
            }    
			break;
		}
	}
	return i;	
}

/*******************************************************************
 *description:
 *	To extract the ret_jslink pack 
 *input:
 *	int packet_length:
 *		the size of body buffer
 *	char *body:
 *		the body of ret_jslink pack
 *	int max_link_num:
 *		max number of js_link array
 *output :
 *	int *offset :
 *		offset extracted from dynamic link in the body
 *	char *js_link:
 *		dynamic javascript link included in the body
 *return value:
 *		 number of return javascript link extracted from body
 **********************************************************************/
int extract_ret_jslink_body(char *body,int packet_length,char (*js_link)[UL_MAX_URL_LEN],
int *offset,char *title,int max_title_len,int max_link_num)
{
	int links_length = 0;
	for(unsigned i=0; i<LINK_LEN_BYTE_NUM;i++)
	{
		links_length = ((links_length)<<8) + (body[LINK_LEN_BYTE-PACK_BODY_BYTE +i]&0xff);
	}	
	
	int title_length=0;
	for(unsigned i=0; i<TITLE_LEN_BYTE_NUM;i++)
	{
		title_length = ((title_length)<<8) + (body[TITLE_LEN_BYTE -PACK_BODY_BYTE+i]&0xff);
	}	

    char *purl_start = body + LINK_LEN_BYTE_NUM + TITLE_LEN_BYTE_NUM;
    char *purl_end = purl_start + links_length-1;

    char *ptitle_start = purl_end +1;
    char *ptitle_end = ptitle_start+ title_length;
    
	if(packet_length != (title_length + links_length+TITLE_LEN_BYTE_NUM + LINK_LEN_BYTE_NUM))
	{
		ul_writelog(UL_LOG_NOTICE,"ret jslink packet:length is not equal.[Packet_len:%d] [Links_len:%d] [Title_len:%d]\r\n",packet_length,links_length,title_length);
		return FAIL;
	}
    
    int i;
    char *pstart = purl_start;
	for(i=0;i<max_link_num;i++)
	{
		char *pend = strchr(pstart,'\r');
		if(pend!= NULL&&pend <= purl_end)	
		{
			char *poffset = strchr(pstart,'\t');
            if(poffset!=NULL)
            {
			    *pend = 0;
			    *poffset = 0;
			    snprintf((char *)(js_link+i),UL_MAX_URL_LEN,"%s",pstart);
			    *(offset+i) = atoi(poffset+1);
			    pstart = pend + 1; 
            }else{
                pstart = pend +1;
            }    
        }else{
            break;
        }    
	}

    title[0] = 0;
    pstart = ptitle_start;
    char *psep = strchr(pstart,'\t');
    *ptitle_end = 0;
    if(psep != NULL)
    {
        pstart = psep+1;
        snprintf(title,max_title_len,"%s",pstart);
    }
	return i;	
}

/*******************************************************************
 *description:
 *	To extract the ret_jslink pack 
 *input:
 *	int packet_length:
 *		the size of body buffer
 *	char *body:
 *		the body of ret_jslink pack
 *	int max_link_num:
 *		max number of js_link array
 *output :
 *	char *html:
 *		html page
 *return value:
 *		 number of return javascript link extracted from body
 **********************************************************************/
int extract_ret_writed_page_body(char *body,char *html, int packet_length)
{
    if(packet_length <0 || html == NULL || body == NULL)
    {
        return FAIL;
    }
    memcpy(html,body,packet_length);
    return OK;
}

/*******************************************************************
 *description:
 *	To get a get_js_pack packet
 *input:
 *	int pack_len:
 *		the size of pack buffer
 *	char *script:
 *		the script needed by js_run_script
 *	int script_len:
 *		size of script buffer
 *	dyn_link_t *dyn_link
 *		the links generated with javascript
 *	int dyn_link_num
 *		the num of dyn_link
 *output :
 *	char *pack :
 *		 the get_js_pack pack
 *return value:
 *		 total size of the pack including the body and head
 **********************************************************************/
int make_run_writed_jslink_pack(char *pack,int pack_len,char *url,char *script,int script_len,dyn_link_t *dyn_link,int *dyn_link_num)
{
	if((unsigned)pack_len<PACK_BODY_BYTE+script_len+strlen(url))
	{
		ul_writelog(UL_LOG_NOTICE,"pack len is illegal.[Pack_len:%d]\r\n",pack_len);
		return FAIL;
	}	
	if(*dyn_link_num>MAX_URL_NUM)	
	{
		ul_writelog(UL_LOG_NOTICE,"too many dyn link number.[Dyn_link_num:%d]\r\n",*dyn_link_num);
		*dyn_link_num = MAX_URL_NUM;
	}
#ifdef DEBUG
    assert(dyn_link!=NULL);
    assert(script!=NULL);
    assert(url!=NULL);
    assert(pack!=NULL);
#endif

	memset(pack,0,pack_len);

	pack[TYPE_BYTE] = RUN_WRITED_PAGE_JSLINK_CMD;
	pack[VER_BYTE] = VER;
    
	
	int left_size = pack_len -PACK_BODY_BYTE;
	int _ret = 0;
	char *temp = &pack[SCRP_BYTE];

	_ret = _copy_text(temp,_ret,left_size,url);
    temp[_ret] = 0;
    _ret ++;
    if(script_len + _ret > left_size){
        ul_writelog(UL_LOG_NOTICE,"script len is too long.script_len:%d url_len:%zu\r\n",script_len,strlen(url));
        return FAIL;
    }
   
    memcpy(&temp[_ret],script,script_len);
    _ret += script_len;
	char buffer[UL_MAX_URL_LEN+10];
    int i;
	for(i =0; i<*dyn_link_num;i++)
	{
//		ul_writelog(UL_LOG_DEBUG,"copy dyn link:[%d]\r\n",i);
		if(left_size<=0)	break;
		snprintf(buffer,UL_MAX_URL_LEN+10,"%s\t%d\r",dyn_link[i].js_link,dyn_link[i].offset);
		_ret = _copy_text(temp,_ret,left_size,buffer);
	}
	int len = _ret+SCRP_BYTE-PACK_BODY_BYTE;
    *dyn_link_num = i;
	
	pack[DYNLINK_NUM_BYTE] = (*dyn_link_num) /256;
	pack[DYNLINK_NUM_BYTE+1] = (*dyn_link_num) %256;
	pack[PACK_LEN_BYTE] = (len >> 24) & 0xff;
	pack[PACK_LEN_BYTE+1] = (len >> 16) & 0xff;
	pack[PACK_LEN_BYTE+2] = (len >> 8) & 0xff;
	pack[PACK_LEN_BYTE+3] = (len ) & 0xff;

    script_len += strlen(url)+1;
	pack[SCRP_LEN_BYTE] = (script_len >> 24) & 0xff;
	pack[SCRP_LEN_BYTE+1] = (script_len >> 16) & 0xff;
	pack[SCRP_LEN_BYTE+2] = (script_len >> 8) & 0xff;
	pack[SCRP_LEN_BYTE+3] = (script_len ) & 0xff;
	
	pack[DYNLINK_LEN_BYTE] = (len-script_len-8 >> 24) & 0xff;
	pack[DYNLINK_LEN_BYTE+1] = (len-script_len -8>> 16) & 0xff;
	pack[DYNLINK_LEN_BYTE+2] = (len -script_len-8 >> 8) & 0xff;
	pack[DYNLINK_LEN_BYTE+3] = (len - script_len-8 ) & 0xff;
	return len+PACK_BODY_BYTE;
}

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int net4_ntop(unsigned int ip, char *ipstr)
{
    struct in_addr in_val;
	
	if (ip <= 0 || ipstr == NULL)	return -1;
	in_val.s_addr = ip;
	if (inet_ntop(AF_INET, &in_val, ipstr, INET_ADDRSTRLEN))
		return OK;
	return FAIL;
}
