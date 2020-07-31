#valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all -v ./apsp -f input/all.fasta/all_ests.fasta.11MB
# Compiling with -p for profiling using oprofile tool of gdb
CC=gcc
APSP_HEADERS_DIR=include
SRCDIR=src
INCLUDE_DIRS= -I$(APSP_HEADERS_DIR)/ -I$(SRCDIR)/
CFLAGS= -Wall
LD_FLAGS=

APSP_OBJS=$(SRCDIR)/apsp_main.o \
          $(SRCDIR)/apsp_read_dir.o \
          $(SRCDIR)/apsp_prefix.o \
          $(SRCDIR)/apsp_isfx.o \
          $(SRCDIR)/sand_sa.o \
          $(SRCDIR)/stack.o \
          $(SRCDIR)/queue.o \
          $(SRCDIR)/kasai_lcp.o \
          $(SRCDIR)/trace.o

ifeq ($(outputfile), y)
    CFLAGS+=-DOVLAP_FILE
endif

ifeq ($(profiling), y)
    CFLAGS+=-pg
    LD_FLAGS+=-pg
else 
    ifeq ($(debug), y)
        CFLAGS+=-g
        LD_FLAGS+=-g
    else
        CFLAGS+=-O3
        LD_FLAGS+=-O3
    endif
endif

ifeq ($(trace), n)
    CFLAGS+=-DWITHOUT_TRACE
endif

ifeq ($(outputmem), y)
    CFLAGS+=-DOVLAP_MEM
endif

%.o:%.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $@ -c $<

apsp:$(APSP_OBJS)
	cd $(SRCDIR)/; make
	gcc $(LD_FLAGS) -o $@ $^

clean:
	cd $(SRCDIR)/; make clean
	rm -rf apsp *.o *~

trace:
	rm -rf trace_apsp.txt trace_backup
