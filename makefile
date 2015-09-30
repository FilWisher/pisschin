CC = gcc
LIBS = -L/usr/lib -lc
CFLAGS = -std=c99 -pedantic

all: options pisschin

options:
	@echo build options:
	@echo "CC = $(CC) $(CFLAGS)"
	
pisschin:
	@echo CC -o $@
	@$(CC) -o $@ $@.c $(LIBS) $(CFLAGS)

clean:
	@echo cleaning
	@rm -f pisschin $(OBJ)
