CC=		gcc
CFLAGS=		-g -Wall -std=gnu99 -Iinclude
LD=		gcc
LDFLAGS=	-Llib
LIBS=		-lcrypto
AR=		ar
ARFLAGS=	rcs
TARGETS=	bin/duplicates

all:		$(TARGETS)

# TODO: Add rules for bin/unit_hash, bin/unit_pair, bin/unit_table, bin/duplicates
src/hash.o: src/hash.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/pair.o: src/pair.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/table.o: src/table.c
	$(CC) $(CFLAGS) -c -o $@ $^

lib/libtable.a: src/hash.o src/pair.o src/table.o
	$(AR) $(ARFLAGS) $@ $^


tests/unit_hash.o: tests/unit_hash.c
	$(CC) $(CFLAGS) -c -o $@ $^ 

test/unit_table.o: tests/unit_table.c
	$(CC) $(CFLAGS) -c -o $@ $^

tests/unit_pair.o: tests/unit_pair.c
	$(CC) $(CFLAGS) -c -o $@ $^

src/duplicates.o: src/duplicates.c
	$(CC) $(CFLAGS) -c -o $@ $^

bin/unit_hash: tests/unit_hash.o lib/libtable.a
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

bin/unit_pair: tests/unit_pair.o lib/libtable.a
	$(LD) $(LDFLAGS) -o $@ $^

bin/unit_table: tests/unit_table.o lib/libtable.a
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

bin/duplicates: src/duplicates.o lib/libtable.a
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

test:			test-hash test-pair test-table test-duplicates test-quiz

test-pair:		bin/unit_pair
	@bin/test_pair.sh

test-hash:		bin/unit_hash
	@bin/test_hash.sh

test-table:		bin/unit_table
	@bin/test_table.sh

test-duplicates:	bin/test_duplicates.sh bin/duplicates
	@bin/test_duplicates.sh

test-quiz:		bin/test_quiz.py answers.json
	@bin/test_quiz.py

clean:
	@echo Cleaning...
	@rm -f $(TARGETS) bin/unit_pair bin/unit_hash bin/unit_table lib/*.a src/*.o tests/*.o *.log *.input

.PHONY:		all test clean
