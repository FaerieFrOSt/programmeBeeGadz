#------------------------------------------------------------------------------
#
#
#       Programme Box
#
#
#------------------------------------------------------------------------------

CC = g++

SUFFIX = .c

PREFIX = .

EXE = a.out

INC = -I$(PREFIX)/libnfc -I$(PREFIX)/include

CFLAGS = -Wall -Werror -Wextra

LIB = $(PREFIX)/libnfc/libnfc.a -lusb

SRCDIR = $(PREFIX)/src

SRC = $(wildcard $(SRCDIR)/*$(SUFFIX))

OBJDIR = $(PREFIX)/obj

OBJ = $(subst $(OBJDIR), $(SRCDIR), $(SRC:$(SUFFIX)=.o))

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(INC) $(LIB)

%.o: %$(SUFFIX)
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(EXE)

re: fclean all
