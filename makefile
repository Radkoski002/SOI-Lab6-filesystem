CC = cc

all:
	$(CC) create.c -o create
	$(CC) copy.c -o copy
	$(CC) load.c -o load
	$(CC) remove.c -o remove
	$(CC) clear.c -o clear
	$(CC) show.c -o show

clean:
	rm -f create copy load remove clear show soiSystem