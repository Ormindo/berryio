# ===== DIRECTORIES =====
OBJ_DIR = bin
SRC_DIR = src
TST_DIR = tests
INC_DIR = inc

# ===== COMPILE OPTIONS =====
CC = arm-linux-gnueabihf-gcc
CFLAGS = -I $(INC_DIR) -Wall -std=c99 -Wextra -g

LD = arm-linux-gnueabihf-gcc
LDFLAGS = -lm

# ===== FILES =====
C_SRC = $(wildcard $(SRC_DIR)/*.c)
H_SRC = $(wildcard $(INC_DIR)/*.h)
O_SRC = $(C_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

C_TST = $(wildcard $(TST_DIR)/*.c)
O_TST = $(C_TST:$(TST_DIR)/%.c=$(OBJ_DIR)/%.o)
TST_BIN = $(C_TST:$(TST_DIR)/%.c=$(OBJ_DIR)/%.out)

# ===== RULES =====
.PHONY: all clean compile tests

all : compile

tests : $(TST_BIN) $(O_TST)

$(OBJ_DIR)/%.out : $(OBJ_DIR)/%.o $(O_SRC)
	$(LD) -o $@ $^

compile : $(O_SRC)

# Compile src files with header
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(INC_DIR)/%.h
	$(CC) -c $(CFLAGS) -o $@ $<

# Compile test files without header
$(OBJ_DIR)/%.o : $(TST_DIR)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean :
	rm -f $(OBJ_DIR)/*.o
	rm -f $(OBJ_DIR)/*.out



