.PHONY: all clean mrproper re

CC		:=	cc
CC_FLAGS	:=	-Wall -Wextra -Werror -Iinc  -Itools/inc -g
CC_MACRO	:=	-DOPTIMIZE
LD_FLAGS	:=
TOOLS_OBJ	:=	bitset.o vector.o buffer.o
OBJ		:=	main.o dfa.o tokendef.o nfa.o regex.o utils.o dfagen.o
## Add Prefix ##
OBJ		:=	$(addprefix src/,$(OBJ))
TOOLS_OBJ	:=	$(addprefix tools/src/,$(TOOLS_OBJ))
## Merge All ##
OBJ		+=	$(TOOLS_OBJ)
TARGET		:=	fgfl

all: $(TARGET);

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LD_FLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CC_FLAGS) $(CC_MACRO)

clean:
	@rm -rf $(OBJ)

mrproper: clean
	@rm -rf $(TARGET)

re: mrproper all;
