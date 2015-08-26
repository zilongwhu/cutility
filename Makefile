objects=src/dual_dir.o src/encode_util.o src/json2pb.o src/pb2json.o src/meminfo.o src/str_utils.o src/fsint.o src/dm_dict.o
.PHONY:all
all: libutils.a
libutils.a: $(objects) src/cJSON.o
	ar crs $@ $^
$(objects): %.o:%.cpp
	g++ -g -Wall -Iinclude -I../lsnet/include -c $<  -o $@
src/cJSON.o: src/cJSON.c
	g++ -g -Wall -Iinclude -I../lsnet/include -c $<  -o $@
clean:
	rm -rf src/*.o
	rm -rf libutils.a 
