directory = $(sort $(dir $(wildcard ./*/.)))

all: $(directory)

$(directory):
	make -C ./$@

.PHONY: $(directory)
