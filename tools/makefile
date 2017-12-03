.PHONY: all clean mrproper re

CC		:=	cc
AR		:=	ar
CC_FLAGS	:=	-Wall -Wextra -Werror -I../inc  -Iinc -g
CC_MACRO	:=	-DOPTIMIZE
OBJ		:=	bitset.o vector.o buffer.o lexer.o
OBJ		:=	$(addprefix src/,$(OBJ))
TARGET		:=	libtools.a

all: $(TARGET);

$(TARGET): $(OBJ)
	$(AR) rs $@ $^

%.o: %.c
	$(CC) -c $< -o $@ $(CC_FLAGS) $(CC_MACRO)

clean:
	@rm -rf $(OBJ)

mrproper: clean
	@rm -rf $(TARGET)

re: mrproper all;
