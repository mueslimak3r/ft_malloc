NAME = libftmalloc.so
CC := gcc

all: $(NAME)

MODULES := src includes
LIBDIRS := libft
LIBS := -Llibft -lft
CFLAGS += -Ilibft/includes -Iincludes -Wall -Werror -Wextra -g -fsanitize=address
MODNAME := module.mk
SRC :=

include $(patsubst %,%/$(MODNAME),$(MODULES))

OBJ :=  $(patsubst %.c,%.o,$(filter %.c,$(SRC)))
DEP :=	$(patsubst %.c,%.d,$(filter %.c,$(SRC)))

-include $(DEP)

$(NAME): $(OBJ)
	make -C libft
	$(CC) $(CFLAGS) -shared -fPIC $(OBJ) $(LIBS) -o $@

%.d : %.c
	@./depend.sh $*.o $(CFLAGS) $< > $@
	@printf '\t%s' "$(CC) $(CFLAGS) -fPIC -c -o $*.o $<" >> $@
	@echo $@ >> all.log

unit_tests: all
	$(CC) $(CFLAGS) -L. -lftmalloc -Llibft -lft unit_tests/*.c

clean:
	make clean -C libft
	rm -f $(OBJ)
	rm -f $(shell cat all.log)
	@rm -f all.log

clean_nolib:
	rm -f $(OBJ)
	rm -f $(shell cat all.log)
	@rm -f all.log

fclean: clean_nolib
	make fclean -C libft
	rm -f $(NAME)
	rm -rf a.out*

re: clean_nolib all

.PHONY: all clean fclean re
