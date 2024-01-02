# makefile to build main program

include platform.mk

SRC_DIR := src/cre_obu
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/cre_obu
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CFLAGS   := -Wall -Iinclude/cre_obu
CPPFLAGS :=
LDFLAGS  := -Llib -Llibext
# LDLIBS   := -llte_v2x
LDLIBS   :=

.PHONY: all clean

all : $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)

