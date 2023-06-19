
CROSS_COMPILE=

AS              = $(CROSS_COMPILE)as
LD              = $(CROSS_COMPILE)ld
CC              = $(CROSS_COMPILE)gcc
CPP             = $(CROSS_COMPILE)g++
AR              = $(CROSS_COMPILE)ar
NM              = $(CROSS_COMPILE)nm
STRIP           = $(CROSS_COMPILE)strip
OBJCOPY         = $(CROSS_COMPILE)objcopy
OBJDUMP         = $(CROSS_COMPILE)objdump
RANLIB          = $(CROSS_COMPILE)ranlib


LIB     = ../release/lib

PLAYERLIBS	= $(LIB)/libplayer.a
PLAYER          = ./player
DEPEND_FILES = ./playerapi.o \
               ./DummySink.o\
               ./rtspClient.o\
               ./BasicTaskSchedulerEpoll.o

CFLAGS	=  -DLINUX
CFLAGS	+=  -fvisibility=hidden
CFLAGS  += -fPIC

DEBUG	= -g -Wall
IFLAGS 	= -I. -I../live/BasicUsageEnvironment/include\
		-I../live/liveMedia/include\
		-I../live/groupsock/include\
		-I../live/UsageEnvironment/include 

LDFLAGS = -L$(LIB)/  \
	-L../live/BasicUsageEnvironment -L../live/liveMedia\
                -L../live/groupsock -L../live/UsageEnvironment\
                -lplayer -lliveMedia\
                -lgroupsock -lBasicUsageEnvironment -lUsageEnvironment \
				-lssl -lcrypto -lpthread -lrt -lstdc++

all:  $(PLAYER)


$(PLAYERLIBS): $(DEPEND_FILES)
	$(AR) $(ARFLAGS) $(PLAYERLIBS) $?
	cp -f playerapi.h ../release/inc
	make -C ../live

player.o : player.c
	$(CC) -c player.c

$(PLAYER): $(PLAYERLIBS) player.o
	$(CC) -o $(PLAYER) $(DEBUG) $(CFLAGS) $(IFLAGS) ./player.o $(LDFLAGS)

clean:
	rm -f $(PLAYERLIBS) $(DEPEND_FILES) ./player.o $(PLAYER) 

.cpp.o:
	$(CPP) -c -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $<
.c.o:
	$(CC) -c -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $<
