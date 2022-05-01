override CFLAGS := -Wall -Werror -std=gnu99 -pedantic -O0 -g -pthread $(CFLAGS)
#-Wall -Werror
override LDLIBS := $(LDLIBS)

fs.o: fs.h fs.c disk.o

disk.o: disk.c disk.h

tls_test_1.o: tls_test_1.c tls.h

test_meg: test_meg.o disk.o
	$(CC) $(LDFLAGS) $+ $(LOADLIBES) $(LDLIBS) -o $@

bitmap: bitmap.o disk.o
	$(CC) $(LDFLAGS) $+ $(LOADLIBES) $(LDLIBS) -o $@

inodetest: inodetest.o disk.o
	$(CC) $(LDFLAGS) $+ $(LOADLIBES) $(LDLIBS) -o $@

new_test: new_test.o disk.o
	$(CC) $(LDFLAGS) $+ $(LOADLIBES) $(LDLIBS) -o $@

test_files=  new_test test_meg #test_disk bitmap inodetest

# Run the test programs
check: $(test_files) 
	bash run_tests.sh $(test_files) 

clean:
	rm -f *.o $(test_files) *~

# .PHONY: clean

# clean:
# 	rm -f tls.o
