#
# if you want the ram-disk device, define this to be the
# size in blocks.
#
#AS86	=as86 -0 -a
#LD86	=ld86 -0
AS86	=as86
LD86	=ld86
#LD86	=ld86 -0 -z

AS	=gcc -m32 -x assembler-with-cpp -Wl,--nmagic,--oformat=a.out-i386-linux
#LD	=gld
LD	= ld --verbose
STRIP   = strip
OBJDUMP = objdump
LDFLAGS	=  -s -x -M -nmagic -T   test1.lds -nostdlib
CC	=gcc $(RAMDISK)
#CFLAGS	=-m32 -Wl,--oformat=a.out-i386-linux -Wall -O -fstrength-reduce -fomit-frame-pointer
CFLAGSX64	=  -Wall -O -fstrength-reduce -fomit-frame-pointer

CPP	=cpp -nostdinc -Iinclude

MTXLIBSRCC = MTX4_2/MTXLIB.src/bio.c MTX4_2/MTXLIB.src/queue.c \
             MTX4_2/MTXLIB.src/getc.s MTX4_2/MTXLIB.src/putc.s MTX4_2/MTXLIB.src/setds.s

.c.s:
	$(CC) $(CFLAGS) \
	-nostdinc -Iinclude -S -o $*.s $<
.s.o:
	$(AS) -c -o $*.o $<
.c.o:
	$(CC) $(CFLAGS) \
	-nostdinc -Iinclude -c -o $*.o $<

all:	Image Image_linux-00-0

#Image: boot/boot tools/system tools/build
#	tools/build boot/boot tools/system  > Image
#	sync
Image: linux-0.00mod/boot/boot linux-0.00mod/tools/system
	genext2fs -b 1440 -d ./mount_point/  Image
#	fallocate  -l 1440k Image
#	mkfs.ext2  -b 1024 -T floppy  Image
	dd if=linux-0.00mod/boot/boot of=Image bs=512 count=1 conv=notrunc
	dd if=linux-0.00mod/tools/system of=Image seek=1 bs=512 conv=notrunc
	sync
run_linux-0.00mod: Image
	qemu-system-i386 -fda Image -no-fd-bootchk


loader:loader_mtx.s
	as86 -o loader_mtx.o loader_mtx.s
	ld86 -d -o loader loader_mtx.o

disk: Image
	dd bs=8192 if=Image of=/dev/fd0
	sync;sync;sync

#tools/build: tools/build.c
#	$(CC) $(CFLAGS) \
#	-o tools/build tools/build.c
tools/build: tools/build.c
	$(CC) $(CFLAGSX64) \
	-o tools/build tools/build.c
#LMC 2021.08.27
#boot/head.o: boot/head.s
#
#tools/system:	boot/head.o
#	$(LD) $(LDFLAGS) boot/head.o  -o tools/system > System.map
#REF:
# https://www.rowleydownload.co.uk/arm/documentation/gnu/ld/Options.html
# -x
# --discard-all
# Delete all local symbols
linux-0.00mod/tools/system: linux-0.00mod/boot/head.s
	gcc -g -std=gnu99  -nostdlib -m32 -march=i386 -ffreestanding -x assembler-with-cpp -Wl,--nmagic,--oformat=binary -o linux-0.00mod/boot/head.o -c $<
	gcc -std=gnu99  -nostdlib -m32 -march=i386 -ffreestanding  -o $@ -Wl,--nmagic,--script=linux-0.00mod/test1.lds,-Map=System.map,--retain-symbols-file=linux-0.00mod/filename.rtd linux-0.00mod/boot/head.o


linux-0.00mod/boot/boot:	linux-0.00mod/boot/boot.s
	$(AS86) -o linux-0.00mod/boot/boot.o linux-0.00mod/boot/boot.s
	$(LD86) -d -o linux-0.00mod/boot/boot linux-0.00mod/boot/boot.o
#	$(LD86) -s -M -o boot/boot boot/boot.o

tools/system_1: boot/head_1.s
	gcc -g -std=gnu99  -nostdlib -m32 -march=i386 -ffreestanding -x assembler-with-cpp -Wl,--nmagic,--oformat=binary -o boot/head_1.o -c $<
	gcc -std=gnu99  -nostdlib -m32 -march=i386 -ffreestanding  -o $@ -Wl,--nmagic,--script=test1bis.lds,-Map=System_1.map,--retain-symbols-file=filename.rtd boot/head_1.o


clean:
	rm -vf Image System.map tmp_make core boot/boot System_1.map
	rm -vf init/*.o tools/system tools/build boot/*.o tools/system_1

cleanall:
	rm -vf Image System.map tmp_make core boot/boot System_1.map
	rm -vf init/*.o linux-0.00mod/tools/system linux-0.00mod/tools/build linux-0.00mod/boot/*.o linux-0.00mod/tools/system_1

cleanrun_linux-0.00mod:
	rm -vf Image System.map tmp_make core boot/boot System_1.map
	rm -vf init/*.o linux-0.00mod/tools/system linux-0.00mod/tools/build linux-0.00mod/boot/*.o linux-0.00mod/tools/system_1

# ----linux-0.00----
Image_linux-0.00: linux-0.00/boot/boot_from_orig_Image linux-0.00/tools/system
	genext2fs -b 1440 -d ./mount_point/  $@
	dd if=linux-0.00/boot/boot_from_orig_Image of=$@ bs=512 count=1 conv=notrunc
	dd if=linux-0.00/tools/system of=$@ seek=1 bs=512 conv=notrunc

linux-0.00/tools/system: linux-0.00/boot/head.s
	gcc -g -std=gnu99  -nostdlib -m32 -march=i386 -ffreestanding -x assembler-with-cpp -Wl,--nmagic,--oformat=binary -o linux-0.00/boot/head.o -c $<
	gcc -std=gnu99  -nostdlib -m32 -march=i386 -ffreestanding  -o $@ -Wl,--nmagic,--script=linux-0.00/test1.lds,-Map=System.map,--retain-symbols-file=linux-0.00/filename.rtd linux-0.00/boot/head.o


linux-0.00/boot/boot:	linux-0.00/boot/boot.s
	$(AS86) -o linux-0.00/boot/boot.o linux-0.00/boot/boot.s
	$(LD86) -s -o linux-0.00/boot/boot linux-0.00/boot/boot.o
#	$(LD86) -s -M -o boot/boot boot/boot.o

run_linux-0.00: Image_linux-0.00
	qemu-system-i386 -fda $< -no-fd-bootchk

cleanrun_linux-0.00:
	rm -vf Image_linux-0.00 System.map tmp_make core boot/boot
	rm -vf init/*.o linux-0.00/tools/system linux-0.00/tools/build linux-0.00/boot/*.o

# --- MTX4_0/ ---

MTX4_0/mtx:MTX4_0/ts.s MTX4_0/t.c
	as86 -o MTX4_0/ts.o MTX4_0/ts.s
	bcc  -o MTX4_0/t.o -c -ansi MTX4_0/t.c
	ld86 -d -o MTX4_0/mtx MTX4_0/ts.o MTX4_0/t.o MTX4_0/mtxlib /usr/lib/bcc/libc.a

run_MTX4_0:MTX4_0/mtx MTX4_0/booter
	cp -v MTX4_0/mtx MTX4_0/mount_point/boot/
	genext2fs  -b 1440 -d MTX4_0/mount_point/  MTX4_0/mFDimage
#	dd if=FDimage.img of=mFDimage count=2 conv=notrunc
	dd if=MTX4_0/booter of=MTX4_0/mFDimage bs=1024 count=1 conv=notrunc
	qemu-system-i386 -fda MTX4_0/mFDimage -no-fd-bootchk

MTX4_0/booter:MTX4_0/mtxFS/bs.s MTX4_0/mtxFS/bc.c
	@echo "compiling booter ..."
	as86 -o MTX4_0/bs.o MTX4_0/mtxFS/bs.s
	bcc -o MTX4_0/bc.o -c -ansi MTX4_0/mtxFS/bc.c
	@echo "linking booter ....."
	ld86 -d -o MTX4_0/booter MTX4_0/bs.o MTX4_0/bc.o /usr/lib/bcc/libc.a


cleanrun_MTX4_0:
	rm -v MTX4_0/*.o MTX4_0/mtx MTX4_0/mount_point/boot/mtx MTX4_0/booter MTX4_0/mFDimage # MTX4_0/loader

# --- MTX4_1/ ---

MTX4_1/mtx:MTX4_1/ts.s MTX4_1/t.c
	as86 -o MTX4_1/ts.o MTX4_1/ts.s
	bcc  -o MTX4_1/t.o -c -ansi MTX4_1/t.c
	ld86 -d -o MTX4_1/mtx MTX4_1/ts.o MTX4_1/t.o MTX4_1/mtxlib /usr/lib/bcc/libc.a

run_MTX4_1:MTX4_1/mtx MTX4_1/booter
	cp -v MTX4_1/mtx MTX4_1/mount_point/boot/
	genext2fs -v -b 1440 -d MTX4_1/mount_point/  MTX4_1/mFDimage
#	dd if=FDimage.img of=mFDimage count=2 conv=notrunc
	dd if=MTX4_1/booter of=MTX4_1/mFDimage bs=1024 count=1 conv=notrunc
	qemu-system-i386 -fda MTX4_1/mFDimage -no-fd-bootchk

MTX4_1/booter:MTX4_1/mtxFS/bs.s MTX4_1/mtxFS/bc.c
	@echo "compiling booter ..."
	as86 -o MTX4_1/bs.o MTX4_1/mtxFS/bs.s
	bcc -o MTX4_1/bc.o -c -ansi MTX4_1/mtxFS/bc.c
	@echo "linking booter ....."
	ld86 -d -o MTX4_1/booter MTX4_1/bs.o MTX4_1/bc.o /usr/lib/bcc/libc.a

cleanrun_MTX4_1:
	rm -v MTX4_1/*.o MTX4_1/mtx MTX4_1/mount_point/boot/mtx MTX4_1/booter MTX4_1/mFDimage

# --- MTX4_2/ ---

MTX4_2/mtx:MTX4_2/ts.s MTX4_2/t.c MTX4_2/mtxlib
	as86 -o MTX4_2/ts.o MTX4_2/ts.s
	bcc  -o MTX4_2/t.o -c -ansi MTX4_2/t.c
	ld86 -d -o MTX4_2/mtx MTX4_2/ts.o MTX4_2/t.o MTX4_2/mtxlib /usr/lib/bcc/libc.a

run_MTX4_2:MTX4_2/mtx MTX4_2/booter
	cp -v MTX4_2/mtx MTX4_2/mount_point/boot/
#	genext2fs -v -b 1440 -d ./mount_point/  mFDimage
	genext2fs -b 1440 -d MTX4_2/mount_point/  MTX4_2/mFDimage
#	dd if=FDimage.img of=mFDimage count=2 conv=notrunc
	dd if=MTX4_2/booter of=MTX4_2/mFDimage bs=1024 count=1 conv=notrunc
	qemu-system-i386 -fda MTX4_2/mFDimage -no-fd-bootchk

MTX4_2/booter:MTX4_2/mtxFS/bs.s MTX4_2/mtxFS/bc.c
	@echo "compiling booter ..."
	as86 -o MTX4_2/bs.o MTX4_2/mtxFS/bs.s
	bcc -o MTX4_2/bc.o -c -ansi MTX4_2/mtxFS/bc.c
	@echo "linking booter ....."
	ld86 -d -o MTX4_2/booter MTX4_2/bs.o MTX4_2/bc.o /usr/lib/bcc/libc.a
MTX4_2/mtxlib:$(MTXLIBSRC)
	cd MTX4_2/MTXLIB.src/ && $(MAKE)

MTX4_2/mFDimage:MTX4_2/mtx MTX4_2/booter
	cp -v MTX4_2/mtx MTX4_2/mount_point/boot/
	genext2fs -b 1440 -d MTX4_2/mount_point/  MTX4_2/mFDimage
	dd if=MTX4_2/booter of=MTX4_2/mFDimage bs=1024 count=1 conv=notrunc

cleanrun_MTX4_2:
	rm -v MTX4_2/*.o MTX4_2/MTXLIB.src/*.o MTX4_2/mtx MTX4_2/mount_point/boot/mtx MTX4_2/booter MTX4_2/mFDimage MTX4_2/mtxlib

