.PHONY: clean install uninstall all

PREFIX ?= /usr/local
CC      = gcc
RUNDIR  = build
BINS    = $(RUNDIR)/main
DLLS    = $(RUNDIR)/libluasophia.so
RM      = rm -f
SRC     = 
DEPS    = $(wildcard deps/*/*.c)
OBJS    = $(DEPS:.c=.o)
CFLAGS  = -std=c99 -Ideps -O3 -Ideps/lua -Ldeps/lua -Ideps/sophia -Ldeps/sophia -g
CFLAGS += -Wall -Werror -Wno-unused-function -Wno-unused-value -Wno-unused-variable
CDLL    = -fpic -shared
CLIB    = -llua -ldl -lm -lsophia -lpthread

all: $(BINS) $(DLLS)

$(BINS): $(SRC) $(OBJS)
	mkdir -p $(RUNDIR)
	$(CC) $(CFLAGS) -o $@ src/$(notdir $(@:.exe=)).c $(OBJS) $(CLIB) $(LDFLAGS)

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
	@cd build ; LD_LIBRARY_PATH=../deps/jansson:../deps/sophia:../deps/lua ./$(notdir $(1))

endef

$(foreach i,$(BINS),$(eval $(call runner,$(i))))