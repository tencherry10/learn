.PHONY: clean install uninstall all

PREFIX ?= /usr/local
CC      = gcc
RUNDIR  = build
BINS    = $(RUNDIR)/main
DLLS    = 
RM      = rm -f
SRC     = $(wildcard src/*.c)
DEPS    = $(wildcard deps/*/*.c)
OBJS    = $(DEPS:.c=.o)
CFLAGS  = -std=c99 -pedantic -Ideps -Isrc -O3
CFLAGS += -Wall -Werror -Wextra -Wno-unused-function -Wno-unused-value -Wno-unused-variable -Wno-unused-result
CFLAGS_BINS = -DTEST_MAIN
CDLL    = -shared
CLIB    = -ldl -lm
LIB_PATH= ../deps/jansson:../deps/sophia:../deps/lua:../../libhost/build/

all: $(BINS) $(DLLS)

$(BINS): $(SRC) $(OBJS)
	mkdir -p $(RUNDIR)
	$(CC) $(CFLAGS) $(CFLAGS_BINS) -o $@ src/$(notdir $(@:.exe=)).c $(OBJS) $(CLIB) $(LDFLAGS)

$(DLLS): $(SRC) $(OBJS)
	mkdir -p $(RUNDIR)
	$(CC) $(CFLAGS) $(CDLL) -o $@ src/$(notdir $(@:.so=)).c $(OBJS) $(CLIB) $(LDFLAGS) 

%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)

install: $(BINS)
	cp -f $(BINS) $(PREFIX)/bin/$(BINS)

uninstall:
	rm -f $(PREFIX)/bin/mon

clean:
	$(RM) -r $(RUNDIR)
	$(RM) $(OBJS)

define runner

$(notdir $(1)): all $(1)
	#@cd build ; strip -s ./$(notdir $(1))
	@cd build ; LD_LIBRARY_PATH=$(LIB_PATH) ./$(notdir $(1))

$(notdir $(1))_mem: all $(1)
	@cd build ; LD_LIBRARY_PATH=$(LIB_PATH) valgrind ./$(notdir $(1))


endef

$(foreach i,$(BINS),$(eval $(call runner,$(i))))

