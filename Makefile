##
# copyright by JakeYang
#


BRANCH=$(shell git branch | grep "*" | awk -F' ' '{print $$2}')
COMMIT=$(shell git log --pretty=format:'%h'|head -1)

PHASCAN_DIR=$(PWD)
SRC_DIR=$(PHASCAN_DIR)/src
SCRIPT_DIR=$(PHASCAN_DIR)/script

ifdef _PC_
SYSROOT_DIR= /
TARGET=main
BUILD_DIR ?= $(PHASCAN_DIR)/build/$(BRANCH)
else
SYSROOT_DIR=$(shell dirname `which arm-angstrom-linux-gnueabi-gcc`)/../../../../armv7a-angstrom-linux-gnueabi
TARGET=main_arm
BUILD_DIR ?= $(PHASCAN_DIR)/build/$(BRANCH)-arm
endif

C_SRCS = $(shell cd $(SRC_DIR) && find . -name "*.c")
SUBDIRS = $(shell cd $(SRC_DIR) && find . -type d)
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SRCS))

INSTALL = install
CROSS_COMPILE ?= arm-angstrom-linux-gnueabi-
CC=$(CROSS_COMPILE)gcc
INC_DIRS = -I $(SYSROOT_DIR)/usr/include/libxml2 \
	   -I $(SYSROOT_DIR)/usr/include/webkit-1.0 \
	   -I $(SYSROOT_DIR)/usr/include/libsoup-2.4 \
	   -I $(SYSROOT_DIR)/usr/include/gtk-2.0	\
	   -I $(SYSROOT_DIR)/usr/lib/gtk-2.0/include \
	   -I $(SYSROOT_DIR)/usr/include/atk-1.0 \
	   -I $(SYSROOT_DIR)/usr/include/cairo \
	   -I $(SYSROOT_DIR)/usr/include/pango-1.0 \
	   -I $(SYSROOT_DIR)/usr/include/gio-unix-2.0/ \
	   -I $(SYSROOT_DIR)/usr/include/pixman-1 \
	   -I $(SYSROOT_DIR)/usr/include/freetype2 \
	   -I $(SYSROOT_DIR)/usr/include/libpng12 \
	   -I $(SYSROOT_DIR)/usr/include/glib-2.0 \
	   -I $(SYSROOT_DIR)/usr/lib/glib-2.0/include  \
	   -I $(SRC_DIR)/dxflib/ \
	   -I $(SRC_DIR)/gdxf/

LDFLAGS= -lpthread \
	 -lgdk-x11-2.0 \
	 -lgtk-x11-2.0 \
	 -latk-1.0 \
	 -lpangoft2-1.0 \
	 -lgdk_pixbuf-2.0 \
	 -lm \
	 -lpangocairo-1.0 \
	 -lcairo \
	 -lgio-2.0 \
	 -lpango-1.0 \
	 -lfreetype \
	 -lfontconfig \
	 -lgobject-2.0 \
	 -lgmodule-2.0 \
	 -lgthread-2.0 \
	 -lrt \
	 -lglib-2.0 \
	 -lwebkitgtk-1.0 \
	 -lsoup-2.4 \
	 -lxml2 \
	 -lfakekey \
	 -rdynamic

CFLAGS = -g -D_REENTRANT -DARM -pthread $(INC_DIRS)

ifdef RELEASE
	CFLAGS = -O2 -D_REENTRANT -DARM -pthread $(INC_DIRS)
endif

VERSION=$(shell $(SCRIPT_DIR)/tag.sh)

VERSION_MAJOR=$(shell echo $(VERSION) | awk -F"." '{print $$1}')
VERSION_MINOR=$(shell echo $(VERSION) | awk -F"." '{print $$2}')
VERSION_MICRO=$(shell echo $(VERSION) | awk -F"." '{print $$3}')

all: prepare build

version:
	@if [ "${VERSION_MICRO}" == "" ];then \
		echo "Not define VERSION_MICRO"; \
		exit 1;\
	fi
	@echo -e "#ifndef __VERSION_H__\n#define __VERSION_H__\n\n"\
	"#define APP_MAJOR ($(VERSION_MAJOR))\n"\
	"#define APP_MINOR ($(VERSION_MINOR))\n"\
	"#define APP_MICRO ($(VERSION_MICRO))\n"\
	"#define GIT_COMMIT \"$(COMMIT)\"\n\n"\
	"#define FPGA_VERSION (2)\n"\
	"\n#endif" > ${SRC_DIR}/version.h

prepare: version
	@mkdir -p $(BUILD_DIR); \
		cd $(BUILD_DIR) ; \
		mkdir -p $(SUBDIRS)

build: $(OBJS)
	$(CC) -o $(BUILD_DIR)/$(TARGET) $(OBJS) $(LDFLAGS)

install:
	scp $(BUILD_DIR)/$(TARGET) root@192.168.1.217:/home/root/
#	$(INSTALL) $(BUILD_DIR)/$(TARGET) /tftp/

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

help:
	@echo make params:
	@echo "_PC_=1    - build pc target"
	@echo "RELEASE=1 - build release target"

