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

	char Buf[1024];

	ret=convert_type(TYPE(CUBE_MANAGE_BASIC),TYPE(CUBE_MANAGE_BASIC),"enum_cube_manage",Buf);

	if(ret>0)
		printf("%s\n",Buf);		
	else
		printf("convert failed!\n");

	ret=convert_subtype(TYPE(CUBE_MANAGE_BASIC),NULL,Buf);

	if(ret>0)
		printf("%s\n",Buf);		
	else
		printf("convert failed!\n");

	ret=convert_record(TYPE_PAIR(CUBE_MANAGE_BASIC,HEAD),Buf);

	if(ret>0)
		printf("%s\n",Buf);		
	else
		printf("convert failed!\n");

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
	}
	return 0;
}
