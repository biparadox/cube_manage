#ifndef CUBE_MANAGE_H
#define CUBE_MANAGE_H

enum dtype_cube_manage
{
	DTYPE_CUBE_MANAGE=0x310,
};
enum subtype_cube_manage
{
	SUBTYPE_CUBE_NODE_INFO=0x01,
	SUBTYPE_CUBE_INSTANCE_INFO,
	SUBTYPE_CUBE_CONF_REQ,
	SUBTYPE_CUBE_CONF_FILE,
};

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
