CC=gcc
APSP_HEADERS_DIR=../include
INCLUDE_DIRS= -I$(COMMON_DIR)/ -I$(APSP_HEADERS_DIR)/ -I.
#CFLAGS= -g -Wall -DWITHOUT_TRACE -pg
#CFLAGS= -Wall -pg -DOVLAP_FILE
CFLAGS= -Wall -O2 -DOVLAP_FILE

APSP_OBJS=apsp_main.o \
          apsp_read_str.o \
          apsp_isfx.o \
          apsp_prefix.o \
          stack.o \
          queue.o \
          sand_sa.o \
          kasai_lcp.o \
          trace.o

%.o:%.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $@ -c $<

default:$(APSP_OBJS)

clean:
	rm -rf *.o *~

trace:
	rm -rf trace_apsp.txt trace_backup
