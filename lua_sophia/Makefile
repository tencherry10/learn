.PHONY: clean install uninstall all

PREFIX ?= /usr/local
CC      = gcc
RUNDIR  = build
BINS    = 
DLLS    = $(RUNDIR)/libluasophia.so
RM      = rm -f
SRC     = 
DEPS    = $(wildcard deps/*/*.c)
OBJS    = $(DEPS:.c=.o)
CFLAGS  = -std=c99 -Ideps -O3 -Ideps/lua -Ldeps/lua -Ideps/sophia -Ldeps/sophia
CFLAGS += -Wall -Werror -Wno-unused-function -Wno-unused-value -Wno-unused-variable
CDLL    = -fpic -shared
CLIB    = -llua -ldl

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

run:
	LD_LIBRARY_PATH=deps/lua $(BINS)
