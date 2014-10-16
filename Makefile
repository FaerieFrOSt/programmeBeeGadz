#------------------------------------------------------------------------------
#
#
#       Programme Box
#
#
#------------------------------------------------------------------------------

CC = gcc

PREFIX = .

EXE = a.out

INC = -I$(PREFIX)/libnfc -I$(PREFIX)/include

CFLAGS = -Wall -Werror -Wextra

LIB = $(PREFIX)/libnfc/libnfc.a -lusb

SRCDIR = $(PREFIX)/src

SRC = $(wildcard $(SRCDIR)/*.c)

OBJDIR = $(PREFIX)/obj

OBJ = $(subst $(OBJDIR), $(SRCDIR), $(SRC:.c=.o))

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(INC) $(LIB)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(EXE)

re: fclean all
