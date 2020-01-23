CC = gcc

thr_create:
	$(CC) -o thr_create thr_create.c -lpthread

thr_shared:
	$(CC) -o thr_shared thr_shared.c -lpthread

proc_shared:
	$(CC) -o proc_shared proc_shared.c -lpthread

proc_manage:
	$(CC) -o proc_manage proc_manage.c -lpthread

ibadcnt:
	$(CC) -o ibadcnt ibadcnt.c

fork:
	$(CC) -o fork fork.c

all: thr_create thr_shared proc_shared proc_manage ibadcnt fork

zip:
	$zip -r cis3110_a1_dmahmood.zip Makefile answers-a1.txt

clean:
	rm thr_create thr_shared proc_shared proc_manage ibadcnt fork