CC := gcc

MODULES := src includes
#LIBS := #-Llibft -lft

CFLAGS +=  -Iincludes -Wall -Werror -Wextra -fPIC
#CFLAGS += -Ilibft/includes
#CFLAGS += -g -fsanitize=address

MODNAME := module.mk
SRC :=

HOST_TYPE :=

ifeq ($(HOSTTYPE),)
	HOST_TYPE := $(shell uname -m)_$(shell uname -s)
else
	HOST_TYPE := $(HOSTTYPE)
endif

NAME := libft_malloc_$(HOST_TYPE).so
LINK_NAME := libft_malloc.so

include $(patsubst %,%/$(MODNAME),$(MODULES))

OBJ :=  $(patsubst %.c,%.o,$(filter %.c,$(SRC)))
DEP :=	$(patsubst %.c,%.d,$(filter %.c,$(SRC)))

all: $(NAME)

-include $(DEP)

$(NAME): $(OBJ)
	#make -C libft
	$(CC) $(CFLAGS) $(OBJ) -shared -o $@
	ln -fs $(NAME) $(LINK_NAME)

%.d : %.c
	@./depend.sh $*.o $(CFLAGS) $< > $@
	@printf '\t%s' "$(CC) $(CFLAGS) -c -o $*.o $<" >> $@
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
	#make fclean -C libft
	rm -f $(NAME)
	rm -f $(LINK_NAME)
	rm -rf a.out*

re: clean_nolib all

.PHONY: all clean fclean re
