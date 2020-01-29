NAME = libftmalloc.so
CC := gcc

all: $(NAME)

MODULES := src includes
LIBDIRS := libft
#LIBS := #-Llibft -lft
LIBS :=
CFLAGS += -Ilibft/includes -Iincludes -Wall -Werror -Wextra -fsanitize=address -g
MODNAME := module.mk
SRC :=

include $(patsubst %,%/$(MODNAME),$(MODULES))

OBJ :=  $(patsubst %.c,%.o,$(filter %.c,$(SRC)))
DEP :=	$(patsubst %.c,%.d,$(filter %.c,$(SRC)))

-include $(DEP)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -shared -o $@

%.d : %.c
	@./depend.sh $*.o $(CFLAGS) $< > $@
	@printf '\t%s' "$(CC) $(CFLAGS) -c -fpic -o $*.o $<" >> $@
	@echo $@ >> all.log

test: all
	$(CC) $(CFLAGS) main.c -L. -lftmalloc -Llibft -lft -o test

clean:
	#make clean -C libft
	rm -f $(OBJ)
	rm -f $(shell cat all.log)
	@rm -f all.log

clean_nolib:
	rm -f $(OBJ)
	rm -f $(shell cat all.log)
	@rm -f all.log

fclean: clean_nolib
	#make fclean -C libft
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
