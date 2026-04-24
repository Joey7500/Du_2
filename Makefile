CC = gcc
CFLAGS = -g -std=c11 -pedantic -Wall -Wextra -fPIC -fsanitize=address
LDFLAGS = -fsanitize=address

OBJS = htab_hash_function.o htab_init.o htab_size.o htab_bucket_count.o htab_find.o htab_lookup_add.o htab_erase.o htab_for_each.o htab_clear.o htab_free.o

all: tac maxwordcount maxwordcount-dynamic libhtab.a libhtab.so

tac: tac.c
	$(CC) $(CFLAGS) tac.c -o tac.out $(LDFLAGS)

libhtab.a: $(OBJS)
	ar rcs libhtab.a $(OBJS)

libhtab.so: $(OBJS)
	$(CC) $(CFLAGS) -shared $(OBJS) -o libhtab.so $(LDFLAGS)

maxwordcount: maxwordcount.c io.c libhtab.a
	$(CC) $(CFLAGS) -o maxwordcount.out maxwordcount.c io.c libhtab.a $(LDFLAGS)

maxwordcount-dynamic: maxwordcount.c io.c libhtab.so
	$(CC) $(CFLAGS) -o maxwordcount-dynamic.out maxwordcount.c io.c libhtab.so $(LDFLAGS)

clean:
	rm -f *.o tac.out maxwordcount.out maxwordcount-dynamic.out libhtab.a libhtab.so
