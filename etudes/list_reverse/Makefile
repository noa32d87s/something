BINNAME = list_reverse
BINDIR = bin/
CFLAGS = -Wall -O0

all: bindir
	gcc $(CFLAGS) $(BINNAME).c -o $(BINDIR)/$(BINNAME) -lrt

bindir:
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR)
