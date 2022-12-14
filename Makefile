BINDIR          = ./bin
TARGET = $(BINDIR)/audioserver
INTDIR	= $(BINDIR)/OBJ

OBJS = 				\
	$(INTDIR)/serverthread.o	\
	$(INTDIR)/audio.o	\
	$(INTDIR)/talk.o	\
	$(INTDIR)/clientthread.o

all:
	make -C source/common	
	make -C source/
romfs:
	make -C source romfs

# clean
clean: cleansvr

cleansvr:
	rm -f $(TARGET)
	rm -f $(OBJS)