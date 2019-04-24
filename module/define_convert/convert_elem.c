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
#include "struct_ops.h"
#include "convert_elem.h"

// add para lib_include
int convert_record_elem(struct struct_elem_attr * elem_desc,char * elem_define);
int convert_get_structdefine(struct struct_elem_attr * elem_desc,char * define);
int convert_record(int type,int subtype, char * record_define)
{
	int ret;
	int i;
	int offset;
	char Buf[128];
	char * typestr;
	char * subtypestr;
	DB_RECORD * db_record;
	STRUCT_NODE * record_template=memdb_get_template(type,subtype);
	if(record_template==NULL)
		return -EINVAL;
	typestr=memdb_get_typestr(type);	
	subtypestr=memdb_get_subtypestr(type,subtype);	
	db_record=memdb_get_recordtype(type,subtype);
	if(db_record==NULL)
	{
		print_cubeerr("no db_record in (%s, %s) record!\n",typestr,subtypestr); 
	}
	
	if(db_record->head.name[0]!=0)
	{
		sprintf(record_define,"typedef struct %.32s{\n",db_record->head.name);
	}
	else
	{
		sprintf(record_define,"typedef struct tagRECORD(%s,%s){\n",typestr,subtypestr);
	}
	offset=Strlen(record_define);
	for(i=0;record_template->struct_desc[i].name!=NULL;i++)
	{
		record_define[offset++]='\t';
		ret=convert_record_elem(&record_template->struct_desc[i],record_define+offset);
		if(ret<0)
			return ret;
		offset+=ret;
		record_define[offset++]='\n';
		record_define[offset]=0;
	}
	sprintf(record_define+offset,"}__attribute__((packed)) RECORD(%s,%s);\n",typestr,subtypestr);
	offset+=Strlen(record_define+offset);
	return offset;	
}

int convert_record_elem(struct struct_elem_attr * elem_desc,char * elem_define)
{
	int offset=0;
	switch(elem_desc->type)
	{
		case CUBE_TYPE_STRING:
			sprintf(elem_define,"char %s[%d];",elem_desc->name,elem_desc->size);
			break;
		case CUBE_TYPE_UUID:
			sprintf(elem_define,"BYTE %s[32];",elem_desc->name);
			break;
		case CUBE_TYPE_INT:
			sprintf(elem_define,"int %s;",elem_desc->name);
			break;
		case CUBE_TYPE_ENUM:
		case CUBE_TYPE_FLAG:
		case CUBE_TYPE_ELEMTYPE:
		case CUBE_TYPE_RECORDTYPE:
		case CUBE_TYPE_RECORDSUBTYPE:
		case TPM_TYPE_UINT32:
			sprintf(elem_define,"UINT32 %s;",elem_desc->name);
			break;
		case CUBE_TYPE_SENUM:
		case CUBE_TYPE_SFLAG:
		case CUBE_TYPE_USHORT:
		case TPM_TYPE_UINT16:
			sprintf(elem_define,"UINT16 %s;",elem_desc->name);
			break;
		case CUBE_TYPE_BENUM:      // an 8-bit enum
		case CUBE_TYPE_BFLAG:      // an 8-bit flag
		case CUBE_TYPE_UCHAR:    // a unsigned octet
		case CUBE_TYPE_BOOL:  
			sprintf(elem_define,"BYTE %s;",elem_desc->name);
			break;
		case CUBE_TYPE_TIME:     // an struct of time_t
		case CUBE_TYPE_LONGLONG: // a 64-bit longlong integer
		case TPM_TYPE_UINT64:
			sprintf(elem_define,"UINT64 %s;",elem_desc->name);
			break;
		case CUBE_TYPE_BINDATA:  // a sequence of octets with fixed size
		case CUBE_TYPE_BITMAP:   // a sequence of octets with fixed size(just like BINDATA),but we use eight bin string (like 01001010) to show them	 
		case CUBE_TYPE_HEXDATA:   // a sequence of octets with fixed size(just like BINDATA),but we use 2 hex string (like ce) to show them	 
			sprintf(elem_define,"BYTE %s[%d];",elem_desc->name,elem_desc->size);
			break;
		case CUBE_TYPE_BINARRAY:   // an array of sequence of octets with fixed size, attr is the sequence's size, size is array's length	 
			sprintf(elem_define,"BYTE %s[%d];",elem_desc->name,elem_desc->size*((int)(elem_desc->ref)));
			break;
		case CUBE_TYPE_UUIDARRAY:   // an array of sequence of octets with fixed size, attr is the sequence's size, size is array's length	 
			sprintf(elem_define,"BYTE %s[%d];",elem_desc->name,elem_desc->size*DIGEST_SIZE);
			break;
		case CUBE_TYPE_ESTRING:  // a variable length string ended with '\0'
		case CUBE_TYPE_DEFSTR:	//a string whose length defined by a forhead element (an uchar, an ushort or a int element), the attr parameter 
		case CUBE_TYPE_DEFSTRARRAY:	//a fixed string' s array whose elem number defined by a forhead element (an uchar, an ushort,a int element or 
			sprintf(elem_define,"char * %s;",elem_desc->name);
			break;
		case CUBE_TYPE_DEFUUIDARRAY:   // an array of sequence of octets with fixed size, attr is the sequence's size, size is array's length	 
		case CUBE_TYPE_DEFINE:	//an octets sequence whose length defined by a forhead element (an uchar, an ushort or a int element), the attr parameter 
		case CUBE_TYPE_JSONSTRING:  // a variable length string encluded in "{}", "[]" or "\"\"" or "\'\'", it is only special in struct_json, other times,
			sprintf(elem_define,"BYTE * %s;",elem_desc->name);
			break;
		case CUBE_TYPE_DEFNAMELIST:   // an array of sequence of octets with fixed size, attr is the sequence's size, size is array's length	 
			sprintf(elem_define,"struct struct_namelist * %s;",elem_desc->name);
			break;
		case CUBE_TYPE_SUBSTRUCT:    // this element describes a new struct in this site, attr points to the description of the new struct
			offset = convert_get_structdefine(elem_desc,elem_define);
			if(offset<0)
				return offset;
			if((elem_desc->size==0) || (elem_desc->size==1))
				sprintf(elem_define+offset,"%s;",elem_desc->name);
			else
				sprintf(elem_define+offset,"%s[%d];",elem_desc->name,elem_desc->size);
			break;
		case CUBE_TYPE_ARRAY:   // this elemen`t use an pointer point to a fixed number element
		case CUBE_TYPE_DEFARRAY:   // this element has no data
			offset = convert_get_structdefine(elem_desc,elem_define);
			if(offset<0)
				return offset;
			sprintf(elem_define+offset," * %s;",elem_desc->name);
			break;
	
        	case CUBE_TYPE_CHOICE:
       			        // it is same as ESTRING	
		case CUBE_TYPE_NODATA:   // this element has no data
		case CUBE_TYPE_USERDEF:
		case CUBE_TYPE_ENDDATA:
			return 0;
		default:
			return -EINVAL;
	}
	return Strlen(elem_define);	
}

int convert_get_structdefine(struct struct_elem_attr * elem_desc, char * struct_define)
{
	DB_RECORD * substruct;
	DB_RECORD * recordtype;
	char * typestr;
	char * subtypestr;
	if(elem_desc==NULL)
		return -EINVAL;	

	char * substruct_name;
	substruct_name=elem_desc->def;

	if(substruct_name==NULL)
	{
		print_cubeerr("convert_get_structdefine:convert elem %s failed!",elem_desc->name);
		return -EINVAL;
	}

		
	recordtype=memdb_find_byname(substruct_name,DB_RECORDTYPE,0);
	if(recordtype!=NULL)
	{
		struct struct_recordtype *record_data = recordtype->record;
	
		typestr=memdb_get_typestr(record_data->type);	
		if(typestr==NULL)
			return -EINVAL;
		subtypestr=memdb_get_subtypestr(record_data->type,record_data->subtype);
		if(subtypestr==NULL)
			return -EINVAL;
	
		sprintf(struct_define,"RECORD(%s,%s) ",typestr,subtypestr);
	}
	else
	{
		sprintf(struct_define,"struct %s ",substruct->head.name);
	}
		
	return Strlen(struct_define);
}

int convert_type(int start_type,int end_type, char * enum_name,char * record_define)
{
	int ret;
	int i;
	int offset;
	int count=0;
	char Buf[128];
	char * typestr;

	if(enum_name!=NULL)
	{
		sprintf(record_define,"enum %s {\n",enum_name);
		offset=Strlen(record_define);
	}
	else
		offset=0;

	for(i=start_type;i<=end_type;i++)
	{
		typestr=memdb_get_typestr(i);
		if(typestr==NULL)
			continue;
		if(offset==0)
		{
			Strncpy(Buf,typestr,128);
			lower_substring(Buf,128,0,0);
			sprintf(record_define,"enum dtype_%s {\n",Buf);
			offset=Strlen(record_define);
		}

		if(count==0)
		{
			sprintf(record_define+offset,"\tTYPE(%s)=0x%x",typestr,i);
		}
		else
		{
			sprintf(record_define+offset,",\n\tTYPE(%s)=0x%x",typestr);
		}
		count++;
		offset+=Strlen(record_define+offset);
	}	
	sprintf(record_define+offset,"\n};");
	offset+=Strlen(record_define+offset);
	return offset;	
}

int convert_subtype(int type,char * enum_name,char * record_define)
{
	int ret;
	int i;
	int offset;
	int count=0;
	char Buf[128];
	char * typestr;
        struct struct_namelist * subtypelist;
        NAME2VALUE * enum_list;
	int lastvalue=0;

	typestr=memdb_get_typestr(type);
	if(enum_name!=NULL)
		sprintf(record_define,"enum %s {\n",enum_name);
	else
	{
		if(typestr==NULL)
			return -EINVAL;
		Strncpy(Buf,typestr,128);
		lower_substring(Buf,128,0,0);
		sprintf(record_define,"enum subtype_%s {\n",Buf);
	}
	offset=Strlen(record_define);
	
        subtypelist=memdb_get_subtypelist(type);
	if(subtypelist==NULL)
		return -EINVAL;
	enum_list=subtypelist->elemlist;

        for(i=0;i<subtypelist->elem_no;i++)
        {
		if(i==0)
			sprintf(record_define+offset,"\tSUBTYPE(%s,%s)=0x%x",
				typestr,enum_list[i].name,enum_list[i].value);
		else if(enum_list[i].value==lastvalue+1)
			sprintf(record_define+offset,",\n\tSUBTYPE(%s,%s)",
				typestr,enum_list[i].name);

		else
			sprintf(record_define+offset,",\n\tSUBTYPE(%s,%s)=0x%x",
				typestr,enum_list[i].name,enum_list[i].value);
		
		lastvalue=enum_list[i].value;
		offset+=Strlen(record_define+offset);
        }
	sprintf(record_define+offset,"\n};");
	offset+=Strlen(record_define+offset);
	return offset;	
}
