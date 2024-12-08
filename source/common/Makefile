#
# Makefile for build using GNU C++(Unified for all Unix)
# The autoconf will not change this file
# The adaption is done by changing ../../config/unix/Make.defines
#
#
##############################################################################
#


include ../config/unix/Make.defines

#if make from ros

ifdef PLAT_DIR
INCLUDE_OPTIONS = -I./../h -I../../h -I. -I$(USR_DIR_OPENSRC)/openssl/include
else
INCLUDE_OPTIONS = -I./../h -I../../h -I. -I../../thirdpart/openssl/include
endif

CXXFLAGS += $(INCLUDE_OPTIONS) -D_DEBUG 


LIBS		=  -lssl -lcrypto 

BINDIR          = ./../../bin
INTDIR	= $(BINDIR)/OBJ
SRCDIR	= ./../common

OBJS = 				\
	$(INTDIR)/doorsbase.o	\
	$(INTDIR)/Base64.o

TARGET = $(OBJS)

#####################################################################
#
all: server

server: checkbindir checkdir $(TARGET)

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
#
clean: cleansvr

cleansvr:
	rm -f $(TARGET)


#####################################################################
# doorsbase files
$(INTDIR)/doorsbase.o: $(SRCDIR)/doorsbase.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRCDIR)/doorsbase.cpp
$(INTDIR)/Base64.o: $(SRCDIR)/Base64.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRCDIR)/Base64.cpp
