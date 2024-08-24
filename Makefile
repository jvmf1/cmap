CC=cc
NAME=cmap
LIBDESTDIR=/usr/local/lib
INCLUDEDESTDIR=/usr/local/include
CFLAGS=-Wall -g

all: lib$(NAME).a lib$(NAME).so

.PHONY: all clean install uninstall clean

$(NAME).o: $(NAME).c
	$(CC) $(CFLAGS) $< -c

lib$(NAME).a: $(NAME).o
	ar cvrs $@ $<

lib$(NAME).so: $(NAME).o
	$(CC) $(CFLAGS) -shared -fPIC $< -o $@

install: lib$(NAME).a lib$(NAME).so
	mkdir -p $(LIBDESTDIR)
	mkdir -p $(INCLUDEDESTDIR)
	cp -f $^ $(LIBDESTDIR)
	cp -f $(NAME).h $(INCLUDEDESTDIR)

example: example.c lib$(NAME).a
	$(CC) $(CFLAGS) $^ -o $@

uninstall:
	rm -f $(LIBDESTDIR)/lib$(NAME).a
	rm -f $(LIBDESTDIR)/lib$(NAME).so
	rm -f $(INCLUDEDESTDIR)/$(NAME).h

clean:
	rm -f *.o *.a *.so *.out example
