override CFLAGS := -Wall -Werror -std=gnu99 -pedantic -O0 -g -pthread $(CFLAGS)
#-Wall -Werror
override LDLIBS := -pthread $(LDLIBS)

fs.o: fs.h fs.c disk.o

disk.o: disk.c disk.h





tls_test_1.o: tls_test_1.c tls.h

#tls_test_1: tls_test_1.o tls.o
#	$(CC) $(LDFLAGS) $+ $(LOADLIBES) $(LDLIBS) -o $@

test_files=

# Run the test programs
check: $(test_files) 
	bash run_tests.sh $(test_files) 

clean:
	rm -f *.o $(test_files) *~

# .PHONY: clean

# clean:
# 	rm -f tls.o