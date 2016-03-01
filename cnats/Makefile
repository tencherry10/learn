.PHONY: clean install uninstall all

PREFIX ?= /usr/local
CC      = gcc
RUNDIR  = build
BINS    = $(RUNDIR)/pubber $(RUNDIR)/subber $(RUNDIR)/pubtest $(RUNDIR)/subtest
DLLS    = 
RM      = rm -f
SRC     = 
DEPS    = $(wildcard deps/*/*.c)
OBJS    = $(DEPS:.c=.o)
CFLAGS  = -std=c99 -Ideps -O3 -Ldeps/cnats
CFLAGS += -Wall -Werror -Wno-unused-function -Wno-unused-value -Wno-unused-variable
CDLL    = -fpic -shared
CLIB    = -lnats -lrt

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
	LD_LIBRARY_PATH=deps/cnats $(RUNDIR)/subber &
	LD_LIBRARY_PATH=deps/cnats $(RUNDIR)/pubber &
