all: paging

clean:
	rm -r page_replace baker_prog_assign4.zip

paging: baker_prog_assign3.c buffer.h my_semaphore.h
	gcc -o page_replace baker_prog_assign4.c

zip: Makefile README.md baker_prog_assign3.c buffer.h my_semaphore.h
	mkdir baker_prog_assign4/
	cp Makefile README.md baker_prog_assign4.c baker_prog_assign3/
	zip -r baker_prog_assign4.zip baker_prog_assign4
	rm -r baker_prog_assign4/
