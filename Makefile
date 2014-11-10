#------------------------------------------------------------------------------
#
#
#       Programme Box
#
#
#------------------------------------------------------------------------------

CC = g++

SUFFIX = .cpp

PREFIX = .

EXE = a.out

LIBMYSQL = $(shell mysql_config --libs)

MYSQLFLAGS = $(shell mysql_config --cflags)

LIBNFC = $(PREFIX)/libnfc/libnfc.a

INC = -I$(PREFIX)/libnfc -I$(PREFIX)/include

CFLAGS = -g -Wall -Werror -Wextra -std=c++11 $(MYSQLFLAGS)

LIB = $(LIBNFC) -lusb $(LIBMYSQL)

SRCDIR = $(PREFIX)/src

SRC = $(wildcard $(SRCDIR)/*$(SUFFIX))

OBJDIR = $(PREFIX)/obj

OBJ = $(subst $(SRCDIR), $(OBJDIR), $(SRC:$(SUFFIX)=.o)) $(OBJDIR)/mifare.o

all: $(LIBNFC) $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(INC) $(LIB)

$(OBJDIR)/%.o: $(SRCDIR)/%$(SUFFIX)
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

$(OBJDIR)/mifare.o: $(SRCDIR)/mifare.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(EXE)

re: fclean all
