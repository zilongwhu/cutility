objects=src/dual_dir.o src/encode_util.o src/json2pb.o src/meminfo.o src/str_utils.o
.PHONY:all
all: libutils.a
libutils.a: $(objects)
	ar crs $@ $^
$(objects): %.o:%.cpp
	g++ -g -Wall -Iinclude -I../lsnet/include -c $<  -o $@
clean:
	rm -rf src/*.o
	rm -rf libutils.a 
