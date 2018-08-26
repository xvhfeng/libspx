.SUFFIXES: .c .o

SRC_PATH = ./src
HEADER_PATH = ./include/
TARGET = 
TARGET_PATH = /usr/local/bin
HEADER_TARGET_PATH = /usr/local/include/

CC = gcc
CFLAG = -pipe -o0  -W -Wall -Werror -g \
		-Wpointer-arith -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wunused-value
INC_PATH = -I/usr/local/include -I/usr/include \
			-I$(HEADER_PATH)
LIB_PATH = -L/usr/local/lib -lev -lpthread
DEFS := 

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

SUB_DIRS := $(shell find $(SRC_PATH) -maxdepth 3 -type d)
SRC_FILES = $(foreach dir,$(SUB_DIRS),$(wildcard $(dir)/*.c))
SHARED_OBJS = $(patsubst %.c,%.o,$(SRC_FILES) )



#SRC_FILE = $(wildcard *.c) \
#			$(wildcard $(SRC_PATH)/*.c)

HEADER_FILE = $(wildcard *.h) \
			$(wildcard $(HEADER_PATH)/*.h)

#SHARED_OBJS = $(patsubst %.c,%.o,$(SRC_FILE) )

all:$(SHARED_OBJS)
	@echo $(SUB_DIRS)
	@echo $(SRC_FILES)
	@echo $(SHARED_OBJS)
	
%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@ $(LIB_PATH) $(INC_PATH) $(DEFS)

#$(TARGET) : $(SHARED_OBJS)
#	$(CC) $(CFLAG) $(SHARED_OBJS) $(LIB_PATH) $(INC_PATH) -o $(TARGET) $(DEFS)

#install:
#	cp -f $(TARGET) $(TARGET_PATH)
#	cp -f $(HEADER_FILE) $(HEADER_TARGET_PATH)
clean:
	rm -f $(SHARED_OBJS) $(TARGET)

#clean:
	#@for dir in $(SPX_SUBDIRS); do make -C $$dir clean|| exit 1; done