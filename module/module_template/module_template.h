#ifndef MODULE_TEMPLATE_H
#define MODULE_TEMPLATE_H

// plugin's init func and kickstart func
int module_template_init(void * sub_proc,void * para);
int module_template_start(void * sub_proc,void * para);

struct module_init_para
{
	char * module_name;
	char * module_path;
}__attribute__((packed));

#endif
