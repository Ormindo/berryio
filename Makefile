OBJ_DIR = bin
SRC_DIR = src
INC_DIR = inc
TARGET = $(OBJ_DIR)/prog

CC = arm-linux-gnueabihf-gcc
CFLAGS = -I $(INC_DIR) -Wall -std=c99 -Wextra -g

LD = arm-linux-gnueabihf-gcc
LDFLAGS = -lm

C_FILES = $(wildcard $(SRC_DIR)/*.c)
H_FILES = $(wildcard $(INC_DIR)/*.h)
O_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all clean compile

all : compile

compile : $(O_FILES)
	$(LD) -o $(TARGET) $^ $(LDFLAGS)

# Compile files with header
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(INC_DIR)/%.h
	$(CC) -c $(CFLAGS) -o $@ $<

# Compile files without header
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean :
	rm -f $(OBJ_DIR)/*.o
	rm -f $(TARGET)



