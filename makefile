# makefile to build main program

include platform.mk

SRC_DIR := src/cre_obu
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/cre_obu
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

#CFLAGS   := -Wall -g -ggdb -lstdc++ -O2 -pipe -feliminate-unused-debug-types -MD -MT -Iinclude/cre_obu -Iinclude/v2x_api
CFLAGS   := -Wall -g -ggdb -lstdc++ -O2 -pipe -Iinclude/cre_obu -Iinclude/v2x_api
CPPFLAGS :=
LDFLAGS  := -Wall -g -ggdb -lstdc++ -O2 -pipe -g -feliminate-unused-debug-types -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed -rdynamic -Llib -Llibext
LDLIBS   := -lrt -pthread -lm
EXTLIBS := -llte_v2x -lasn1oer -lasn1per -lasn1rt -lwpmapi -lviicsec -laerolinkPKI -lmisbehaviorReport -lcrypto

.PHONY: all clean

all : $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(EXTLIBS) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)

