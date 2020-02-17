
CC = g++ -I ../lib -L ../lib
stems = $(patsubst %.cpp,%,$(wildcard *.cpp))
defs = $(wildcard *.cpp)
headers := $(defs:.cpp=.h)
objs := $(defs:.cpp=.o)
obj_dir = ../obj
objs := $(addprefix $(obj_dir)/, $(objs))

all: $(objs)# out
out:
	@echo stems: $(stems)
	@echo defs: $(defs)
	@echo headers: $(headers)
	@echo objs: $(objs)

$(obj_dir)/%.o : %.cpp
	@#echo $^
	$(CC) -c -o $@ $<

$(objs): | $(obj_dir)

$(obj_dir):
	mkdir $(obj_dir)

.PHONY: all out $(obj_dir)/%.o
