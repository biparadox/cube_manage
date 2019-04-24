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

char Buf[4096];
static char * output_file;

int proc_types_message(void *sub_proc,void * recv_msg);
int define_convert_init(void * sub_proc, void * para)
{
	int ret;
	// add yorself's module init func here
    	struct init_struct * init_para=para;
    	if(para==NULL)	 
		output_file=NULL;
	else
	{
		output_file=Dalloc0(Strlen(init_para->output_file)+1,NULL);
		Strcpy(output_file,init_para->output_file);
	}
	return 0;
}

int _output_convert(char * convert_str)
{
	if(output_file==NULL)
	{
		printf("%s",convert_str);
	}
	else
	{
		FILE * fp=fopen(output_file,"a+");
		if(fp==NULL)
			return -EIO;
		fprintf(fp,"%s",convert_str);
		fclose(fp);
	}	
	return Strlen(convert_str);
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
	int i,j;
	int ret;
	printf("begin proc types msg \n");

	type=message_get_type(message);
	subtype=message_get_subtype(message);

	RECORD(MESSAGE,TYPES) * types_pair;
	
	i=0;
	j=0;

	ret=message_get_record(message,&types_pair,i++);
	if(ret<0)
		return ret;
	while(types_pair!=NULL)
	{
		
		ret=convert_type(types_pair->type,types_pair->type,NULL,Buf);

		if(ret>0)
		{
			_output_convert(Buf);
			_output_convert("\n");
		}
		else
			printf("convert failed!\n");
		if(types_pair->subtype==0)
		{

			ret=convert_subtype(types_pair->type,NULL,Buf);

			if(ret>0)
			{
				_output_convert(Buf);
				_output_convert("\n");
			}
			else
				printf("convert failed!\n");
		}

		if(types_pair->subtype!=0)
		{
			ret=convert_record(types_pair->type,types_pair->subtype,Buf);
			if(ret>0)
			{
				_output_convert(Buf);
				_output_convert("\n");
			}
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
			for(j=0;j<subtypelist->elem_no;j++)
			{
				ret=convert_record(types_pair->type,subtypelist->elemlist[j].value,Buf);
				if(ret>0)
				{
					_output_convert(Buf);
					_output_convert("\n");
				}
			}
			
		}

		ret=message_get_record(message,&types_pair,i++);
		if(ret<0)
			return ret;
	}
	return ret;
}
