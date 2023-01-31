# Makefile for audioserver 
# Billy Shi.
# 2022/10/28
#
##############################################################################
#

include ../../config/unix/Make.defines

#INCLUDE_OPTIONS = -I./../h -I ../../h -I. -I/usr/local/include/mysql 
#INCLUDE_OPTIONS = -I./../h -I ../../h -I. -I/usr/local/ssl/include 
CFLAGS += -I./../h -I../../h -I. -I../../../thirdpart/openssl/include  -I../../../thirdpart/mosquitto/lib/cpp
CFLAGS += -I../../../thirdpart/mosquitto/include
CFLAGS += -I ./include -I./include/HPR -I./include/ezviz/ezDevSDK 
LDFLAGS += -L ./mipsel-openwrt-linux/lib/dynamic -L ./mips_r3000/ -L ../../../thirdpart/openssl 
LDFLAGS += -L ./lib -lHCEBase -lhpr -lezDevSDK_boot -lezDevSDK_Common_Module -lez_model -lmicrokernel -lezDevSDK_talk
LDFLAGS += -lez_model -lezDevSDK_sub
LDFLAGS += -lezDevSDK_alarm -lezDevSDK_base_func -lezDevSDK_boot -lezDevSDK_cloud
LDFLAGS +=-lezDevSDK_comm
LDFLAGS +=-lezDevSDK_door_lock
LDFLAGS +=-lezDevSDK_leave_message
LDFLAGS +=-lezDevSDK_secure
LDFLAGS +=-lezDevSDK_smart_control
LDFLAGS +=-lezDevSDK_special
LDFLAGS +=-lezDevSDK_thirdparty
LDFLAGS +=-lpthread
LDFLAGS +=-lmosquittopp
LDFLAGS +=-lmosquitto
#LDFLAGS += /home/cmf/lib/libmosquitto.so.1
#LDFLAGS += /home/cmf/lib/cpp/libmosquittopp.so.1
LDFLAGS += ./lib/libmbedtls.a ./lib/libmbedcrypto.a ./lib/libmbedx509.a ./mipsel-openwrt-linux/lib/libsrt.a ./mipsel-openwrt-linux/lib/libdevp2pcore.a



CXXFLAGS += $(INCLUDE_OPTIONS) -DUNIX -D_DEBUG -O2 -s -Wall -Wstrict-prototypes -std=c++11 $(CFLAGS)
#LIBS		= -L/usr/local/lib/mysql -lmysqlclient
ifdef PLAT_DIR
LIBS		= -lssl -lcrypto  -L$(USR_DIR_OPENSRC)/openssl
else
LIBS		= -lssl -lcrypto  -L$(USR_DIR_OPENSRC)/openssl_x86
endif

BINDIR          = ./../../../bin
INTDIR	= $(BINDIR)/OBJ
SRCDIR	= ./../audioserver

OBJS = 				\
	$(INTDIR)/serverthread.o	\
	$(INTDIR)/audio.o	\
	$(INTDIR)/talk.o	\
	$(INTDIR)/clientthread.o

BIND_OBJS =			\
	$(INTDIR)/doorsbase.o

TARGET = $(BINDIR)/audioserver

#####################################################################
#
all: server

server: checkbindir checkdir $(TARGET) 

#####################################################################
# Check dir
checkbindir:
	@if test ! -d $(BINDIR) ; \
		then \
	mkdir $(BINDIR) ; \
	fi


checkdir: 
	@if test ! -d $(INTDIR) ; \
	then \
		mkdir $(INTDIR) ; \
	fi

#####################################################################
# clean
clean: cleansvr

cleansvr:
	rm -f $(TARGET)
	rm -f $(OBJS)

#####################################################################
# (TARGET)
$(TARGET) : $(OBJS) $(BIND_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(BIND_OBJS) $(LIBS) -lpthread 


#####################################################################
# (OBJS) 
$(INTDIR)/serverthread.o: $(SRCDIR)/serverthread.cpp
	$(CXX) $(CXXFLAGS) -g -c -o $@ $(SRCDIR)/serverthread.cpp
$(INTDIR)/clientthread.o: $(SRCDIR)/clientthread.cpp
	$(CXX) $(CXXFLAGS) -g -c -o $@ $(SRCDIR)/clientthread.cpp
$(INTDIR)/audio.o: $(SRCDIR)/audio.cpp
	$(CXX) $(CXXFLAGS) -g -c -o $@ $(SRCDIR)/audio.cpp
$(INTDIR)/talk.o: $(SRCDIR)/talk.cpp
	$(CXX) $(CXXFLAGS) -g -c -o $@ $(SRCDIR)/talk.cpp

####################################################################
#(BIND_OBJS)
$(BIND_OBJS):
	cd ../common;	\
	make;

romfs:
	${STRIP} -o $(ROOTFS_DIR)/sbin/audioserver $(TARGET)  
	mkdir -p $(ROOTFS_DIR)/usr/lib
	cp ./mipsel-openwrt-linux/lib/dynamic/*.so $(ROOTFS_DIR)/usr/lib
	cp ./lib/*.so $(ROOTFS_DIR)/usr/lib
	cp ./lib/libmos* -a $(ROOTFS_DIR)/usr/lib
	cp ./mips_r3000/*.so $(ROOTFS_DIR)/usr/lib
	cp ./asctrl.sh $(ROOTFS_DIR)/sbin
