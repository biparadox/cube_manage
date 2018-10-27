char * Makefile_template[] = {
 "FLAGS= -g -shared -o ",
 "INCLUDE= -I$(CUBELIBPATH)/include/", 
 "CFLAGS= -g -c -fPIC -DUSER_MODE $(INCLUDE)",
 "CC=gcc",
 "CCP=g++",
 "PLUGIN=##module_name##",  // module's name
 "PLUGIN_OBJ=",
 "TEST_LIB= -L$(CUBELIBPATH)/lib -lmessage -lmemdb -lconnector -lstruct -lcrypto_func -lpthread",
 " ",
 "main: lib$(PLUGIN).so",
 "lib$(PLUGIN).so:  $(PLUGIN).o $(PLUGIN_PROC_OBJ) $(INIT_DB_OBJ)",
 "	$(CC) $(FLAGS) lib$(PLUGIN).so $(PLUGIN).o $(PLUGIN_OBJ) $(TEST_LIB)",
 "	mv lib$(PLUGIN).so $(CUBE_SYS_PLUGIN)",
 "	cp $(PLUGIN).cfg $(CUBE_SYS_PLUGIN)",
 "$(PLUGIN).o: $(PLUGIN).c $(PLUGIN).h",
 "	$(CC) $(CFLAGS) $(PLUGIN).c",
 "clean:",
 "	rm -f lib$(PLUGIN).so",
 "	rm  *.o *~",
 NULL
};

char * Makefile_app_include =  "INCLUDE= -I$(CUBESYSPATH)/include/ -I$(CUBELIBPATH)/include -I$(CUBEAPPPATH)/include";

char * module_cfg_template[] = {
	"{",
	"\"libname\":\"##module_name##\",",
	"\"type\":\"MONITOR\",",
	"\"dynamic_lib\":\"lib##module_name##.so\",",
	"\"init\":\"##module_name##_init\",",
	"\"start\":\"lib##module_name##_start\",",
	"}",	
	NULL
};

char * module_header_template = {
	"#ifndef ##MODULE_NAME##_H", 
	"#define ##MODULE_NAME##_H", 
	" ",
	" ",
	"int ##module_name##_init (void * sub_proc, void * para);",
	"int ##module_name##_start (void * sub_proc, void * para);",

	"#endif",
	NULL
};

char * module_src_header_template = {
	"#include <unistd.h>",
	"#include <stdio.h>",
	"#include <stdlib.h>",
	"#include <errno.h>",
	"#include <limits.h>",
	"#include <sys/time.h>",
	" ",
	"#include \"data_type.h\"",
	"#include \"alloc.h\"",
	"#include \"memfunc.h\"",
	"#include \"basefunc.h\"",
	"#include \"struct_deal.h\"",
	"#include \"crypto_func.h\"",
	"#include \"memdb.h\"",
	"#include \"message.h\"",
	"#include \"ex_module.h\"",
	"#include \"sys_func.h\"",
	"#include \"##module_name##.h\"",
	"##module_parameter##(lib_include)",
	NULL
};
