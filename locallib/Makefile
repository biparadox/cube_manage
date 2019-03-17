dirs= src/string_func
all:
	$(foreach N,$(dirs),make -C $(N);)
clean:
	rm -f bin/*.so 
	$(foreach N,$(dirs),make clean -C $(N);)


