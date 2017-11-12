.PHONY: all clean mrproper re
CC		:=	cc
CC_FLAGS	:=	-Wall -Wextra -Werror -Iinc -O0
CC_MACRO	:=	-DOPTIMIZE
LD_FLAGS	:=
OBJ		:=	set.o vector.o main.o tokendef.o regex.o\
			nfa.o dfa.o utils.o out_gen.o

OBJ		:=	$(addprefix src/,$(OBJ))
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
