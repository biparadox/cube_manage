#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <dirent.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/evp.h>
#include <tss/tddl_error.h>
#include <tss/tcs_error.h>
#include <tss/tspi.h>

#include "data_type.h"
#include "errno.h"
#include "alloc.h"
#include "string.h"
#include "basefunc.h"
#include "json.h"
#include "struct_deal.h"
#include "crypto_func.h"
#include "memdb.h"
#include "message.h"
#include "ex_module.h"
#include "sys_func.h"

#include "tesi.h"
#include "file_struct.h"
#include "instance_manage.h"

#include "../../include/cube_manage.h"

int proc_build_confdir(void * sub_proc,void *recv_msg);
int proc_send_nodeinfo(void * sub_proc,void *recv_msg);
int proc_send_conffile(void * sub_proc,void *recv_msg);
int proc_write_conffile(void * sub_proc,void *recv_msg);

int instance_manage_init(void * sub_proc,void * para)
{
	int ret;
	char local_uuid[DIGEST_SIZE];
	

	// prepare the slot sock
	return 0;
}

int instance_manage_start(void * sub_proc,void * para)
{
	int ret;
	int retval;
	void * recv_msg;
	void * send_msg;
	void * context;
	int i;
	int type;
	int subtype;
	int state;
	BYTE uuid[DIGEST_SIZE];

	char local_uuid[DIGEST_SIZE];
	char proc_name[DIGEST_SIZE];
	
	ret=proc_share_data_getvalue("uuid",local_uuid);
	ret=proc_share_data_getvalue("proc_name",proc_name);

	print_cubeaudit("begin instance manager!\n");

	ret=proc_build_confdir(sub_proc,NULL);
	
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
		if((type==DTYPE_MESSAGE)&&
			(subtype==SUBTYPE_CONN_SYNI))
		{
			ret=proc_send_nodeinfo(sub_proc,recv_msg);
		}
		else if((type ==DTYPE_CUBE_MANAGE)&&
			(subtype==SUBTYPE_CUBE_CONF_REQ))
		{
			ret=proc_send_conffile(sub_proc,recv_msg);
		}
		else if((type ==DTYPE_CUBE_MANAGE)&&
			(subtype==SUBTYPE_CUBE_CONF_FILE))
		{
			ret=proc_write_conffile(sub_proc,recv_msg);
		}

	}
	return 0;
}

int proc_build_confdir(void * sub_proc,void *recv_msg)
{
	int ret;
	struct cube_node_info * node_info;
	struct cube_instance_info * instance_info;	
	char buf[DIGEST_SIZE*8];
	int i=0;
	char local_uuid[DIGEST_SIZE];
	char proc_name[DIGEST_SIZE];
	
	ret=proc_share_data_getvalue("uuid",local_uuid);

	instance_info=memdb_get_first_record(DTYPE_CUBE_MANAGE,SUBTYPE_CUBE_INSTANCE_INFO);
	while(instance_info!=NULL)
	{
		sprintf(buf,"mkdir %s",instance_info->instance);
		system(buf);
		i++;
		instance_info=memdb_get_next_record(DTYPE_CUBE_MANAGE,SUBTYPE_CUBE_INSTANCE_INFO);
	}
	return i;
}
int proc_send_nodeinfo(void * sub_proc,void *recv_msg)
{
	int ret;
	struct cube_node_info * node_info;
	struct cube_instance_info * instance_info;
	void * send_msg;	
	int i=0;
	char local_uuid[DIGEST_SIZE];
	ret=proc_share_data_getvalue("uuid",local_uuid);
	
	send_msg=message_create(DTYPE_CUBE_MANAGE,SUBTYPE_CUBE_NODE_INFO,NULL);
	if(send_msg==NULL)
		return -EINVAL;

	node_info=memdb_get_first_record(DTYPE_CUBE_MANAGE,SUBTYPE_CUBE_NODE_INFO);
	while(node_info!=NULL)
	{	
		i++;
		Memcpy(node_info->uuid,local_uuid,DIGEST_SIZE);
		message_add_record(send_msg,node_info);
		node_info=memdb_get_next_record(DTYPE_CUBE_MANAGE,SUBTYPE_CUBE_NODE_INFO);
	}
	ex_module_sendmsg(sub_proc,send_msg);
	return i;
	
}

int proc_send_conffile(void * sub_proc,void *recv_msg)
{
	int ret;
	struct cube_node_info * node_info;
}

int proc_write_conffile(void * sub_proc,void *recv_msg)
{
	int ret;
	struct cube_node_info * node_info;
}
