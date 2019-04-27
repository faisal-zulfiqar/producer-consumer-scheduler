CC=gcc

scheduler:
	$(CC) -o random random.c
	./random
	$(CC) -o scheduler scheduler.c -pthread -w

clean:
	rm -f random scheduler task_list.txt