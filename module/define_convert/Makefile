FLAGS= -g -shared -o 
INCLUDE= -I$(CUBESYSPATH)/include -I$(CUBELIBPATH)/include -I$(CUBEAPPPATH)/include
CFLAGS= -g -c -fPIC -DUSER_MODE $(INCLUDE)
CC=gcc
CCP=g++
PLUGIN=define_convert
PLUGIN_OBJ= convert_elem.o
TEST_LIB= -L$(CUBELIBPATH)/lib -lmessage -lmemdb -lconnector -lstruct -lcrypto_func -lpthread
LOCALLIB=-L$(CUBEAPPPATH)/locallib/bin -lstring_func
 
main: lib$(PLUGIN).so
lib$(PLUGIN).so:  $(PLUGIN).o $(PLUGIN_PROC_OBJ) $(INIT_DB_OBJ) $(PLUGIN_OBJ)
	$(CC) $(FLAGS) lib$(PLUGIN).so $(PLUGIN).o $(PLUGIN_OBJ) $(TEST_LIB) $(LOCALLIB)
	mv lib$(PLUGIN).so $(CUBE_SYS_PLUGIN)
	cp $(PLUGIN).cfg $(CUBE_SYS_PLUGIN)
$(PLUGIN).o: $(PLUGIN).c $(PLUGIN).h
	$(CC) $(CFLAGS) $(PLUGIN).c
convert_elem.o: convert_elem.c
	$(CC) $(CFLAGS) convert_elem.c
clean:
	rm -f lib$(PLUGIN).so
	rm  *.o *~
