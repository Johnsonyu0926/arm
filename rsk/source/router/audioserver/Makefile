# Makefile for audioserver 
# Miksa Wu.
# 2020/10/28
#
##############################################################################
#

# Compiler options
CXXFLAGS += -I./../h -I../../h -I. -I../../../thirdpart/openssl/include  -I../../../thirdpart/mosquitto/lib/cpp
CXXFLAGS += -I../../../thirdpart/mosquitto/include
CXXFLAGS += -I ./include -I./include/HPR -I./include/ezviz/ezDevSDK 
LDFLAGS += -L ./mipsel-openwrt-linux/lib/dynamic -L ./mips_r3000/ -L ../../../thirdpart/openssl 
LDFLAGS += -L ./lib -lHCEBase -lhpr -lezDevSDK_boot -lezDevSDK_Common_Module -lez_model -lmicrokernel -lezDevSDK_talk
LDFLAGS += -lez_model -lezDevSDK_sub
LDFLAGS += -lezDevSDK_alarm -lezDevSDK_base_func -lezDevSDK_boot -lezDevSDK_cloud
LDFLAGS += -lezDevSDK_comm
LDFLAGS += -lezDevSDK_door_lock
LDFLAGS += -lezDevSDK_leave_message
LDFLAGS += -lezDevSDK_secure
LDFLAGS += -lezDevSDK_smart_control
LDFLAGS += -lezDevSDK_special
LDFLAGS += -lezDevSDK_thirdparty
LDFLAGS += -lpthread
LDFLAGS += -lmosquittopp
LDFLAGS += -lmosquitto
LDFLAGS += ./lib/libmbedtls.a ./lib/libmbedcrypto.a ./lib/libmbedx509.a ./mipsel-openwrt-linux/lib/libsrt.a ./mipsel-openwrt-linux/lib/libdevp2pcore.a

CXXFLAGS += -DUNIX -D_DEBUG -O2 -s -Wall -Werror -std=c++11

# Output directories
BINDIR          = ./../../../bin
INTDIR          = $(BINDIR)/OBJ
SRCDIR          = ./../audioserver

# Object files
OBJS =          \
    $(INTDIR)/serverthread.o    \
    $(INTDIR)/easylogging++.o   \
    $(INTDIR)/audio.o           \
    $(INTDIR)/talk.o            \
    $(INTDIR)/clientthread.o

BIND_OBJS =     \
    $(INTDIR)/doorsbase.o

# Target
TARGET = $(BINDIR)/audioserver

#####################################################################
#
all: server

server: checkbindir checkdir $(TARGET) 

#####################################################################
# Check directories
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
# Clean
clean: cleansvr

cleansvr:
    rm -f $(TARGET)
    rm -f $(OBJS)

#####################################################################
# Build target
$(TARGET) : $(OBJS) $(BIND_OBJS)
    $(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(BIND_OBJS) $(LIBS) -lpthread 

#####################################################################
# Object files 
$(INTDIR)/serverthread.o: $(SRCDIR)/serverthread.cpp
    $(CXX) $(CXXFLAGS) -g -c -o $@ $(SRCDIR)/serverthread.cpp
$(INTDIR)/easylogging++.o: $(SRCDIR)/easylogging++.cc
    $(CXX) $(CXXFLAGS) -g -c -o $@ $(SRCDIR)/easylogging++.cc
$(INTDIR)/clientthread.o: $(SRCDIR)/clientthread.cpp
    $(CXX) $(CXXFLAGS) -g -c -o $@ $(SRCDIR)/clientthread.cpp
$(INTDIR)/audio.o: $(SRCDIR)/audio.cpp
    $(CXX) $(CXXFLAGS) -g -c -o $@ $(SRCDIR)/audio.cpp
$(INTDIR)/talk.o: $(SRCDIR)/talk.cpp
    $(CXX) $(CXXFLAGS) -g -c -o $@ $(SRCDIR)/talk.cpp

####################################################################
# Build common objects
$(BIND_OBJS):
    cd ../common; \
    make;

romfs:
    ${STRIP} -o $(ROOTFS_DIR)/sbin/audioserver $(TARGET)  
    mkdir -p $(ROOTFS_DIR)/usr/lib
    cp ./mipsel-openwrt-linux/lib/dynamic/*.so $(ROOTFS_DIR)/usr/lib
    cp ./lib/*.so $(ROOTFS_DIR)/usr/lib
    cp ./lib/libmos* -a $(ROOTFS_DIR)/usr/lib
    cp ./mips_r3000/*.so $(ROOTFS_DIR)/usr/lib
    cp ./asctrl.sh $(ROOTFS_DIR)/sbin
    cp ./vol.sh $(ROOTFS_DIR)/sbin
    cp ./conv.sh $(ROOTFS_DIR)/sbin
    cp ./ttsPlay.sh $(ROOTFS_DIR)/sbin
