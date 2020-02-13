NAME = hypernet
CC := clang++

all: $(NAME)

MODULES := src/user src/server src/unit
CFLAGS += -Iincludes -Wall -Werror -Wextra -g -fsanitize=address
MODNAME := module.mk
SRC :=

include $(patsubst %,%/$(MODNAME),$(MODULES))

OBJ :=  $(patsubst %.cpp,%.o,$(filter %.cpp,$(SRC)))
DEP :=	$(patsubst %.cpp,%.d,$(filter %.cpp,$(SRC)))

-include $(DEP)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $@

%.d : %.c
	@./depend.sh $*.o $(CFLAGS) $< > $@
	@printf '\t%s' "$(CC) $(CFLAGS) -c -o $*.o $<" >> $@
	@echo $@ >> all.log

clean:
	rm -f $(OBJ)
	rm -f $(shell cat all.log)
	@rm -f all.log

clean_nolib:
	rm -f $(OBJ)
	rm -f $(shell cat all.log)
	@rm -f all.log

fclean: clean_nolib
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
