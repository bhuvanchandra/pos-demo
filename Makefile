# a simple makefile to (cross) compile the GTK+ example gtk-inoutgpio.c
# needs an externel cross toolchain and uses the libraries as provided in the rootfs

# set the path to the target libraries
OECORE_TARGET_SYSROOT ?= ${HOME}/toradex/linux-prebuild-images/Colibri_T20_LinuxImageV2.5/rootfs/

# set the prefix for the cross compiler
CROSS_COMPILE ?= ${HOME}/gcc-linaro/bin/arm-linux-gnueabihf-

# if CROSS_COMPILE is not empty look up the libraries in the rootfs provided for the target
ifneq ($(strip $(CROSS_COMPILE)),)
  PKG-CONFIG = pkg-config
  LIBROOT = -L$(OECORE_TARGET_SYSROOT)usr/lib -Wl,-rpath-link,$(OECORE_TARGET_SYSROOT)usr/lib -L$(OECORE_TARGET_SYSROOT)lib -Wl,-rpath-link,$(OECORE_TARGET_SYSROOT)lib
  ARCH_CFLAGS = -march=armv7-a -fno-tree-vectorize -mthumb-interwork -mfloat-abi=hard -mtune=cortex-a9
# This configuration uses the buildsystems headers, don't emit warnings about that
  ARCH_CFLAGS += -Wno-poison-system-directories
# The library version control with the Codesourcery GCC 4.4.1 arm-2009q3 somtimes fails with the following linker error
# undefined reference to `__longjmp_chk@GLIBC_2.11'
# adding libc explicitely cures the issue
  LIBROOT += -Wl,--allow-shlib-undefined
# append .arm to the object files and binaries, so that native and cross builds can live side by side
  BIN_POSTFIX = .arm
else
  PKG-CONFIG = pkg-config
  LIBROOT =
  ARCH_CFLAGS =
  BIN_POSTFIX =
endif

# toolchain binaries
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
STRIP = $(CROSS_COMPILE)strip
RM = rm -rf

# flags for compiler
CFLAGS += -Wall -export-dynamic -g `$(PKG-CONFIG) --cflags gtk+-2.0` $(ARCH_CFLAGS)

# flags for linker only
LDFLAGS += $(LIBROOT) `$(PKG-CONFIG) --libs gtk+-2.0`

# list of all object files
OBJS = pos-elinux.o co-proc.o
# list of additional libraries needed
LIBS =
# name of the resulting executable
PROG = pos

all: $(PROG)

$(PROG): $(OBJS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS) $(LDFLAGS)
	$(STRIP) $@

%.o$(BIN_POSTFIX): %.c Makefile
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	$(RM) $(OBJS) $(PROG)

install:
	cp ./pos /usr/bin
	cp ./meta-data/* /

.PHONY: all clean install
