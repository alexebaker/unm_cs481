all: buffer

clean:
	rm -r buffer baker_prog_assign3.zip

buffer: baker_prog_assign3.c buffer.h my_semaphore.h
	gcc -lpthread -o buffer baker_prog_assign3.c

zip: Makefile README.md baker_prog_assign3.c buffer.h my_semaphore.h
	mkdir baker_prog_assign3/
	cp Makefile README.md baker_prog_assign3.c buffer.h my_semaphore.h baker_prog_assign3/
	zip -r baker_prog_assign3.zip baker_prog_assign3
	rm -r baker_prog_assign3/
