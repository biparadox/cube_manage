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

#include "substring_func.h"
#include "cube_manage.h"
#include "module_template.h"
#include "template_file.h"


struct module_init_para * my_init_para;
int module_template_init(void * sub_proc,void * para)
{
	int ret;
	// add youself's plugin init func here
	if(para==NULL)
		return -EINVAL;
	my_init_para = para;
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


	Memset(&module_define,0,sizeof(module_define));

	Strncpy(module_define.name,my_init_para->module_name,DIGEST_SIZE);
	module_define.type=MOD_TYPE_MONITOR;
	module_define.src_path=dup_str(my_init_para->module_path,256);

	proc_Makefile_gen(sub_proc,&module_define);	
	proc_module_cfgfile_gen(sub_proc,&module_define);	
	proc_module_headerfile_gen(sub_proc,&module_define);	
	proc_module_srcfile_gen(sub_proc,&module_define);	

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
/*
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
*/
	}
	return 0;
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

	Strncpy(filepath,module_define->src_path,512);
	Strcat(filepath,"/");
	Strcat(filepath,module_define->name);
	ret=mkdir(filepath,0777);
		
	Strcpy(filename,filepath);
	Strcat(filename,"/");
	Strcat(filename,"Makefile");
	
	fp=fopen(filename,"w+");
	if(fp==NULL)
		return -EIO;
	
	for(i=0;Makefile_template[i]!=NULL;i++)
	{
		Strncpy(filepath,Makefile_template[i],512);
		if(find_substring(filepath,512,"##module_name##",0)>0)
		{
			replace_substring(filepath,512,"##module_name##",0,module_define->name,32);
		}		
		fprintf(fp,"%s\n",filepath);
	}
	fclose(fp);
}

int proc_module_cfgfile_gen(void * sub_proc, void * para)
{
	char filename[512];
	char filepath[512];
	FILE * fp;
	int ret;
	int i,j;
	if(para==NULL)
		return -EINVAL;
	RECORD(CUBE_MANAGE_MODULE,DEFINE) * module_define =para;	

	Strncpy(filepath,module_define->src_path,512);
	Strcat(filepath,"/");
	Strcat(filepath,module_define->name);
	ret=mkdir(filepath,0666);
	if(ret<0)
		print_cubeerr("Create module dir %s failed!\n",filepath);
		
	Strcpy(filename,filepath);
	Strcat(filename,"/");
	Strcat(filename,module_define->name);
	Strcat(filename,".cfg");
	
	fp=fopen(filename,"w+");
	if(fp==NULL)
		return -EIO;
	
	for(i=0;module_cfg_template[i]!=NULL;i++)
	{
		Strncpy(filepath,module_cfg_template[i],512);
		if(find_substring(filepath,512,"##module_name##",0)>0)
		{
			replace_substring(filepath,512,"##module_name##",0,module_define->name,32);
		}		
		fprintf(fp,"%s\n",filepath);
	}
	fclose(fp);
}

int proc_module_headerfile_gen(void * sub_proc, void * para)
{
	char filename[512];
	char filepath[512];
	char Buf[64];
	FILE * fp;
	int ret;
	int i,j;
	if(para==NULL)
		return -EINVAL;
	RECORD(CUBE_MANAGE_MODULE,DEFINE) * module_define =para;	

	Strncpy(filepath,module_define->src_path,512);
	Strcat(filepath,"/");
	Strcat(filepath,module_define->name);
	ret=mkdir(filepath,0666);

	Strncpy(Buf,module_define->name,32);
	upper_substring(Buf,32,0,0);
		
	Strcpy(filename,filepath);
	Strcat(filename,"/");
	Strcat(filename,module_define->name);
	Strcat(filename,".h");
	
	fp=fopen(filename,"w+");
	if(fp==NULL)
		return -EIO;
	
	for(i=0;module_header_template[i]!=NULL;i++)
	{
		Strncpy(filepath,module_header_template[i],512);
		if(find_substring(filepath,512,"##module_name##",0)>0)
		{
			replace_substring(filepath,512,"##module_name##",0,module_define->name,64);
		}		
		if(find_substring(filepath,512,"##MODULE_NAME##",0)>0)
		{
			replace_substring(filepath,512,"##MODULE_NAME##",0,Buf,64);
		}		
		fprintf(fp,"%s\n",filepath);
	}
	fclose(fp);
}

int proc_module_srcfile_gen(void * sub_proc, void * para)
{
	char filename[512];
	char filepath[512];
	FILE * fp;
	int ret;
	int i,j;
	if(para==NULL)
		return -EINVAL;
	RECORD(CUBE_MANAGE_MODULE,DEFINE) * module_define =para;	

	Strncpy(filepath,module_define->src_path,512);
	Strcat(filepath,"/");
	Strcat(filepath,module_define->name);
	ret=mkdir(filepath,0666);
		
	Strcpy(filename,filepath);
	Strcat(filename,"/");
	Strcat(filename,module_define->name);
	Strcat(filename,".c");
	
	fp=fopen(filename,"w+");
	if(fp==NULL)
		return -EIO;
	
	for(i=0;module_src_template[i]!=NULL;i++)
	{
		Strncpy(filepath,module_src_template[i],512);
		ret=find_substring(filepath,512,"##module_parameter##",0);
		if(ret>0)
		{
			char para[32];
			int start_site=ret+Strlen("##module_parameter##")-1;
			ret=getpara_from_substring(filepath+start_site,512-start_site,'(',')',para);
			if(ret<0)
				fprintf(fp,"\n");
			else
				fprintf(fp,"// add para %s\n",para); 
			continue;
		}
		if(find_substring(filepath,512,"##module_name##",0)>0)
		{
			replace_substring(filepath,512,"##module_name##",0,module_define->name,64);
		}		
		fprintf(fp,"%s\n",filepath);
	}
	fclose(fp);
}
