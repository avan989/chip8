SRC_DIR = ./src
HDR_DIR = ./include
LIB_DIR = /Library/Frameworks/SDL2.framework/Headers
FRAMEWORKS = /Library/Frameworks
SRC = $(wildcard ${SRC_DIR}/*.c)
CFLAGS = -Wall -I${HDR_DIR} -I${LIB_DIR} -F${FRAMEWORKS} -framework SDL2
CC = gcc

main:  
	${CC} -o chip8 ${SRC} ${CFLAGS}