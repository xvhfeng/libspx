.SUFFIXES: .c .o

ROOTPATH=../../
TARGET=allocator_test
CC = gcc
MAKE = make
RM = -rm -rf
CFLAG = -pipe -o0  -W -Wall -Werror -g \
		-Wpointer-arith -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wunused-value
INC_PATH = -I/usr/local/include -I/usr/include 
LIB_PATH = -L/usr/local/lib -lev -lpthread
IGNORE_DIRS := include bin
DEFS := 

include ../../makefile

@echo SPX_INCPATH

SYS := $(strip $(shell uname -s | tr '[:upper:]' '[:lower:]'))
ifeq ($(SYS), linux)
	DEFS += -DSpxLinux
else
	ifeq ($(SYS),unix)
		DEFS += -DSpxUnix
	else
		ifeq ($(SYS),darwin)
			DEFS += -DSpxMac
		endif
	endif
endif

BITS := $(findstring 64,$(shell uname -m ))
ifeq (64, $(BITS))
	DEFS += -DSpx64
else
	ifeq (32, $(BITS))
		DEFS += -DSpx32
	else
		DEFS += -DSpx32
	endif
endif

CURR_DIR_SRC_FILE = $(wildcard *.c) 

#HEADER_FILE = $(wildcard *.h) \
#			$(wildcard $(SPX_HEADER_PATH)/*.h)

CURR_DIR_SHARED_OBJS = $(patsubst %.c,%.o,$(CURR_DIR_SRC_FILE) )

SUBDIRS := $(shell find . -maxdepth 3 -type d)
SUBDIRS := $(filter-out $(IGNORE_DIRS),$(SUBDIRS))

ALL_SRC_FILES = $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))
ALL_SHARED_OBJS = $(patsubst %.c,%.o,$(ALL_SRC_FILES) )

#all: clean $(target)
all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@ $(LIB_PATH) $(INC_PATH) $(DEFS)

$(TARGET): $(ALL_SHARED_OBJS)
	for dir in $(SUBDIRS); \
	do $(MAKE) -C $$dir all || exit 1; \
	done
	$(CC) $(CFLAG) $(ALL_SHARED_OBJS) $(LIB_PATH) $(INC_PATH) -o $(TARGET) $(DEFS)

clean:
	@for dir in $(SUBDIRS); do make -C $$dir clean|| exit 1; done
	$(RM) $(CURR_DIR_SHARED_OBJS) $(TARGET) *.bak *~

.PHONY: all clean
