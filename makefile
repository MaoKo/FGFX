.PHONY: all clean mrproper re

CC		:=	cc
INCLUDE		:=	fgfl/inc tools/inc
CC_FLAGS	:=	-Wall -Wextra -Werror $(addprefix -I,$(INCLUDE)) -g
CC_MACRO	:=	-DOPTIMIZE
LD_FLAGS	:=

include tools/makefile
TOOLS_OBJ	:=	$(addprefix tools/,$(TOOLS_OBJ))

include fgfl/makefile
FGFL_OBJ	:=	$(addprefix fgfl/,$(FGFL_OBJ))

TARGET		:=	efgfl

all: $(TARGET);

$(TARGET): $(TOOLS_OBJ) $(FGFL_OBJ)
	$(CC) $^ -o $@ $(LD_FLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CC_FLAGS) $(CC_MACRO)

clean:
	@rm -rf $(TOOLS_OBJ)
	@rm -rf $(FGFL_OBJ)

mrproper: clean
	@rm -rf $(TARGET)

re: mrproper all;
