##
# copyright by JakeYang
#

SHELL=/bin/bash
BRANCH=$(shell git branch | grep "*" | awk -F' ' '{print $$2}')
COMMIT=$(shell git log --pretty=format:'%h'|head -1)

PHASCAN_DIR=$(PWD)
SRC_DIR=$(PHASCAN_DIR)/src
TOOL_DIR=$(PHASCAN_DIR)/tool

ifdef _PC_
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

ifndef _PC_
CROSS_COMPILE ?= arm-angstrom-linux-gnueabi-
CFLAG_ARM=-DARM
endif

INC_DIRS = $(shell PKG_CONFIG_PATH=$(SYSROOT_DIR)/usr/lib/pkgconfig PKG_CONFIG_SYSROOT_DIR=$(SYSROOT_DIR) pkg-config  --cflags gtk+-2.0 webkit-1.0 ) -I $(SRC_DIR)/dxflib -I $(SRC_DIR)/gdxf -I $(PHASCAN_DIR)/include/lua5.1/
LDFLAGS = $(shell PKG_CONFIG_PATH=$(SYSROOT_DIR)/usr/lib/pkgconfig PKG_CONFIG_SYSROOT_DIR=$(SYSROOT_DIR) pkg-config --libs gtk+-2.0 webkit-1.0) -lpthread -ljpeg -lxml2 -lpng12 -lX11 -lm -lfakekey -L$(PHASCAN_DIR)/lib -llua

CFLAGS = -g -D_REENTRANT $(CFLAG_ARM) $(INC_DIRS)

ifdef RELEASE
	CFLAGS = -O2 -D_REENTRANT $(CFLAG_ARM) $(INC_DIRS)
endif

CC=$(CROSS_COMPILE)gcc

VERSION=$(shell git describe --tags `git rev-list --tags --max-count=1`)

VERSION_MAJOR=$(shell echo $(VERSION) | awk -F"." '{print $$1}')
VERSION_MINOR=$(shell echo $(VERSION) | awk -F"." '{print $$2}')
VERSION_MICRO=$(shell echo $(VERSION) | awk -F"." '{print $$3}')

all: version build

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
	"\n#endif" > ${SRC_DIR}/version.h

build: $(OBJS)

	$(CC) -o $(BUILD_DIR)/$(TARGET) $(OBJS) $(LDFLAGS)

install:
	scp $(BUILD_DIR)/$(TARGET) root@192.168.1.217:/home/root/
#	$(INSTALL) $(BUILD_DIR)/$(TARGET) /tftp/

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -c -o $@ $<

help:
	@echo make params:
	@echo "_PC_=1            - build pc target"
	@echo "RELEASE=1         - build release target"

