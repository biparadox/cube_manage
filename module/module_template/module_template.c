#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <dirent.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>

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

#include "cube_manage.h"
#include "module_template.h"
#include "template_file.h"


int module_template_init(void * sub_proc,void * para)
{
	int ret;
	// add youself's plugin init func here
	return 0;
}

int module_template_start(void * sub_proc,void * para)
{
	int ret;
	void * recv_msg;
	int i;
	int type;
	int subtype;

	RECORD(CUBE_MANAGE_MODULE,DEFINE) module_define;

	if(para==NULL)
		return -EINVAL;
	module_init_para * my_init_para = para;

	Memset(module_define,0,sizeof(module_define));

	Strncpy(module_define.name,my_init_para->module_name,DIGEST_SIZE);
	module_define.type=MOD_TYPE_MONITOR;
	module_define.src_path=dup_str(module_path,256);

	proc_Makefile_gen(sub_proc,&module_define);	

	for(i=0;i<3000*1000;i++)
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
		if((type==DTYPE_MESSAGE)&&(subtype==SUBTYPE_CTRL_MSG))
		{
			ret=proc_ctrl_message(sub_proc,recv_msg);
			if(ret==MSG_CTRL_EXIT)
				return MSG_CTRL_EXIT;
		}
		else
		{
			proc_echo_message(sub_proc,recv_msg);
		}
	}
	return 0;
};

int proc_module_create(void * sub_proc,void * message)
{
	int type;
	int subtype;
	int i;
	int ret;
	printf("begin proc module_create \n");

	type=message_get_type(message);
	subtype=message_get_subtype(message);

	void * new_msg;
	void * record;
	new_msg=message_create(type,subtype,message);
	
	i=0;

	ret=message_get_record(message,&record,i++);
	if(ret<0)
		return ret;
	while(record!=NULL)
	{
		message_add_record(new_msg,record);
		ret=message_get_record(message,&record,i++);
		if(ret<0)
			return ret;
	}

	ex_module_sendmsg(sub_proc,new_msg);

	return ret;
}

int proc_Makefile_gen(void * sub_proc, void * para)
{
	char filename[512];
	char filepath[512];
	FILE * fp;
	int ret;
	int i,j;
	if(para==NULL)
		return -EINVAL;
	RECORD(CUBE_MANAGE_MODULE,DEFINE) * module_define =para;	

	Strncpy(filepath,module_define->src_path);
	Strcat(filepath,"/");
	Strcat(filepath,module_define->name);
	ret=mkdir(filepath,0666);
	if(ret<0)
		return ret;
		
	Strcpy(filename,filepath);
	Strcat(filename,"/");
	Strcat(filename,"Makefile");
	
	fp=fopen(filename,"w+");
	if(fp==NULL)
		return -EIO;
	
	for(i=0;Makefile_template[i]!=NULL;i++)
	{
		
	}
	
}
