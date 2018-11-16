#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <sys/time.h>
 
#include "data_type.h"
#include "alloc.h"
#include "memfunc.h"
#include "basefunc.h"
#include "struct_deal.h"
#include "crypto_func.h"
#include "memdb.h"
#include "message.h"
#include "ex_module.h"
#include "sys_func.h"

#include "substring_func.h"
#include "cube_manage.h"
#include "define_convert.h"
#include "convert_elem.h"
// add para lib_include

int proc_types_message(void *sub_proc,void * recv_msg);
int define_convert_init(void * sub_proc, void * para)
{
	int ret;
	// add yorself's module init func here
	return 0;
}
int define_convert_start(void * sub_proc, void * para)
{
	int ret;
	void * recv_msg;
	int type;
	int subtype;
	// add yorself's module exec func here


	while(1)
	{
		usleep(time_val.tv_usec);
		ret=ex_module_recvmsg(sub_proc,&recv_msg);
		if(ret<0)
			continue;
		if(recv_msg==NULL)
			continue;
		type=message_get_type(recv_msg);
		subtype=message_get_subtype(recv_msg);
		if(!memdb_find_recordtype(type,subtype))
		{
			printf("message format (%d %d) is not registered!\n",
			message_get_type(recv_msg),message_get_subtype(recv_msg));
			continue;
		}
		if((type==TYPE(MESSAGE))&&(subtype==SUBTYPE(MESSAGE,TYPES)))
		{
			ret=proc_types_message(sub_proc,recv_msg);
		}

	}
	return 0;
}
int proc_types_message(void * sub_proc,void * message)
{
	int type;
	int subtype;
	int i;
	int ret;
	char Buf[1024];
	printf("begin proc types msg \n");

	type=message_get_type(message);
	subtype=message_get_subtype(message);

	RECORD(MESSAGE,TYPES) * types_pair;
	
	i=0;

	ret=message_get_record(message,&types_pair,i++);
	if(ret<0)
		return ret;
	while(types_pair!=NULL)
	{
		
		ret=convert_type(types_pair->type,types_pair->type,"enum_cube_manage",Buf);

		if(ret>0)
			printf("%s\n",Buf);		
		else
			printf("convert failed!\n");
		if(types_pair->subtype==0)
		{

			ret=convert_subtype(types_pair->subtype,NULL,Buf);

			if(ret>0)
				printf("%s\n",Buf);		
			else
				printf("convert failed!\n");
		}

		if(types_pair->subtype!=0)
		{
			ret=convert_record(type,subtype,Buf);
			if(ret>0)
				printf("%s\n",Buf);		
			else
				printf("convert failed!\n");
		}
		else
		{
			struct struct_namelist * subtypelist;
			subtypelist=memdb_get_subtypelist(types_pair->type);
			if(subtypelist==NULL)
			{
				printf("get error subtypelist!\n");
				return -EINVAL;
			}	
			for(i=0;i<subtypelist->elem_no;i++)
			{
				ret=convert_record(type,subtypelist->elemlist[i].value,Buf);
				if(ret>0)
					printf("%s\n",Buf);		
			}
			
		}

		ret=message_get_record(message,&types_pair,i++);
		if(ret<0)
			return ret;
	}
	return ret;
}
