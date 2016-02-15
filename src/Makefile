#add by lzk
-include lzk/Makefile
-include compile.config

CC=gcc
PROG_NAME=main
INCS=${header_files} ${h_files_lzk}

SRCS=${source_files} ${c_files_lzk}

# 从xx.c 文件得到 xx.o 文件
OBJS=${SRCS:.c=.o}

#编译GTK程序时要用到的库
LIBS=webkit-1.0 gthread-2.0
#---- 用户修改区域 结束

# -O2
export CFLAGS=`pkg-config --cflags ${LIBS}` -O0 -DX86 -Wall -g -I $(PWD)/dxflib/ -I $(PWD)/gdxf
export LDFLAGS=`pkg-config --libs ${LIBS}` -lfakekey -Wall -g -ljpeg -lxml2 -lpng12 -lX11 -lm

all: ${PROG_NAME}

#${PROG_NAME}:${OBJS} focallaw_x86_64.o
${PROG_NAME}:${OBJS} focallaw.o
#	@echo 'Building target: $@'
	${CC} -o ${PROG_NAME} ${OBJS} ${LDFLAGS} focallaw.o
#	${CC} -o ${PROG_NAME} ${OBJS} ${LDFLAGS} focallaw_x86_64.o
#	@echo 'Finished building target: $@'
#	@echo ' '
#注意：上边”${CC}" 的前边有一个TAB键，而不是空格

#如果有头文件进行修改，则自动编译源文件
${OBJS}:${INCS}

#.c.o:
#	${CC} -c $<  ${CFLAGS}
%.o:%.c
#	@echo 'Building file: $<'
	${CC} -o $@ -c $< ${CFLAGS}
#	@echo 'Finished building: $<'
#	@echo ' '
	
.PHONY:clean
clean:
	rm -f ${PROG_NAME} ${OBJS}

rebuild: clean all
