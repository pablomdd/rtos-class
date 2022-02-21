EXE:=test
EXE1:=test_g
EXE2:=test_uclib.xtn
EXE3:=test_g_uclib.xtn
CC = gcc
# CC = tcc
MYCFLAGS = -DTEST1 -I./include

LD = ld
TARGET_PREFIX = /usr/x86_64-linux-uclibc
STARTUP_FILES = ${TARGET_PREFIX}/usr/lib/crt1.o ${TARGET_PREFIX}/usr/lib/crti.o
END_FILES = ${TARGET_PREFIX}/usr/lib/crtn.o
LINKED_FILES = ${STARTUP_FILES} ${OBJS} ${END_FILES}
LIBS = -lc
USER_CFLAGS += -static -nostdlib ${LINKED_FILES} -L${TARGET_PREFIX}/usr/lib ${LIBS}

all:$(EXE1) $(EXE) $(EXE2) $(EXE3)
default:$(EXE)

.c.o:
	$(CC) -I./include -Wall -c $<
$(EXE):hello.c
	$(CC) -I./include $< -o $@
$(EXE1):g.c hello.c
#	$(CC) -DTEST1 -I./include -o $@ $^
	$(CC) $(MYCFLAGS) -o $@ $^
$(EXE2):hello.o
	# codigo uClibc enlazado statico
	$(LD) $< ${USER_CFLAGS} -o $@
$(EXE3):hello.o g.o 
	# codigo uClibc
	$(LD) $< ${USER_CFLAGS} -o $@
clean:
	rm -v $(EXE1) $(EXE) $(EXE2) $(EXE3)
	
	

