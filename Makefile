CC=cc
NAME=cmap
LIBDESTDIR=/usr/local/lib
INCLUDEDESTDIR=/usr/local/include
CFLAGS=-ffunction-sections -fdata-sections

all: lib$(NAME).a

.PHONE: all clean install uninstall clean

$(NAME).o: $(NAME).c $(NAME).h
	$(CC) $(CFLAGS) $< -c -o $@

lib$(NAME).a: $(NAME).o
	ar rcs lib$(NAME).a $<

install: lib$(NAME).a
	mkdir -p $(LIBDESTDIR)
	mkdir -p $(INCLUDEDESTDIR)
	cp -f lib$(NAME).a $(LIBDESTDIR)
	cp -f $(NAME).h $(INCLUDEDESTDIR)

example: example.c lib$(NAME).a
	$(CC) $< lib$(NAME).a -Wall -o $@

uninstall:
	rm -f $(LIBDESTDIR)/lib$(NAME).a
	rm -f $(INCLUDEDESTDIR)/$(NAME).h

clean:
	rm -f *.o *.a example
