#ifndef CUBE_MANAGE_H
#define CUBE_MANAGE_H

enum dtype_cube_manage
{
	TYPE(CUBE_MANAGE_BASIC)=0x310,
	TYPE(CUBE_MANAGE_DATA),
	TYPE(CUBE_MANAGE_MODULE),
	TYPE(CUBE_MANAGE_ENTITY),
	TYPE(CUBE_MANAGE_ROUTE),
	TYPE(CUBE_MANAGE_CONN),
	TYPE(CUBE_MANAGE_CHANNEL),
	TYPE(CUBE_MANAGE_ACTION)
};
enum subtype_cube_manage_basic
{
	SUBTYPE(CUBE_MANAGE_BASIC,HEAD)=0x01,
	SUBTYPE(CUBE_MANAGE_BASIC,REPLY)
};
enum subtype_cube_manage_data
{
	SUBTYPE(CUBE_MANAGE_DATA,FORMAT)=0x01,
	SUBTYPE(CUBE_MANAGE_DATA,ELEM),
	SUBTYPE(CUBE_MANAGE_DATA,TYPE_GROUP),
	SUBTYPE(CUBE_MANAGE_DATA,TYPE),
	SUBTYPE(CUBE_MANAGE_DATA,SUBTYPE),
	SUBTYPE(CUBE_MANAGE_DATA,RECORDTYPE),
	SUBTYPE(CUBE_MANAGE_DATA,RECORD)
};
enum subtype_cube_manage_module
{
	SUBTYPE(CUBE_MANAGE_MODULE,DEFINE)=0x01,
	SUBTYPE(CUBE_MANAGE_MODULE,PLUGIN),
	SUBTYPE(CUBE_MANAGE_MODULE,RECORD),
	SUBTYPE(CUBE_MANAGE_MODULE,MESSAGE)
};
enum subtype_cube_manage_entity
{
	SUBTYPE(CUBE_MANAGE_ENTITY,NODE_INFO)=0x01,
	SUBTYPE(CUBE_MANAGE_ENTITY,INSTANCE_INFO)
};
enum enum_manage_action
{
	GET=0x01,
	POST,
	HEAD,
	DELETE,
	REPLY=0x10	
};

enum enum_manage_class
{
	CUBE_MANAGE_DATA=0x01,
	CUBE_MANAGE_ENTITY
};

enum flag_cube_elem_attr
{
        FLAG_ELEM_ATTR_POINTER=0x01,
        FLAG_ELEM_ATTR_DEFINE=0x02,
        FLAG_ELEM_ATTR_ARRAY=0x04,
        FLAG_ELEM_ATTR_ENUM=0x10,
        FLAG_ELEM_ATTR_FLAG=0x20,
        FLAG_ELEM_ATTR_SUBSET=0x40,
        FLAG_ELEM_ATTR_VALUE=0x80,
        FLAG_ELEM_ATTR_NUM=0x100,
        FLAG_ELEM_ATTR_BOOL=0x200,
        FLAG_ELEM_ATTR_NAMELIST=0x400,
        FLAG_ELEM_ATTR_REF=0x800,
        FLAG_ELEM_ATTR_FIXSIZE=0x1000,
        FLAG_ELEM_ATTR_SPECIAL=0x2000,
        FLAG_ELEM_ATTR_EMPTY=0x80000,

};

typedef struct cube_manage_head
{
	enum enum_manage_action manage_action; 	
	enum enum_manage_class manage_class; 	
	BYTE serialID[32];
}__attribute__((packed)) RECORD(CUBE_MANAGE_BASIC,HEAD);

typedef struct cube_manage_reply
{
	enum enum_manage_action manage_action; 	
	enum enum_manage_class manage_class; 	
	int  return_code;
	BYTE serialID[32];
}__attribute__((packed)) RECORD(CUBE_MANAGE_BASIC,REPLY);

typedef struct cube_data_format
{
	char name[DIGEST_SIZE];
	int  size;
	int  attr;  // flag with flag_cube_elem_attr
	char * desc;
}__attribute__((packed)) RECORD(CUBE_MANAGE_DATA,FORMAT);
typedef struct cube_manage_module_define
{
	char name[DIGEST_SIZE];
	int type;
	char * module_path;
	char * src_path;
	char * desc;
}__attribute__((packed)) RECORD(CUBE_MANAGE_MODULE,DEFINE);
struct cube_node_info
{
	BYTE uuid[DIGEST_SIZE];
	char manager[DIGEST_SIZE];
	int  instance_num;
	BYTE * instances;
}__attribute__((packed));

struct cube_instance_info
{
	BYTE uuid[DIGEST_SIZE];
	char instance[DIGEST_SIZE];
	char manager[DIGEST_SIZE];
	char * path;
}__attribute__((packed));


struct cube_config_request
{
	BYTE uuid[DIGEST_SIZE];
	char instance[DIGEST_SIZE];
	char manager[DIGEST_SIZE];
	char * conf_file;
}__attribute__((packed));

struct cube_config_file
{
	BYTE uuid[DIGEST_SIZE];
	char instance[DIGEST_SIZE];
	char manager[DIGEST_SIZE];
	char * conf_file;
	BYTE digest[DIGEST_SIZE];
}__attribute__((packed));
#endif
