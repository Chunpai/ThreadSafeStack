All: serial.out parallel.out

serial.out: serial.c timing.c GetNumbers.c
	gcc serial.c timing.c GetNumbers.c -o serial.out

parallel.out: parallel.c timing.c GetNumbers.c
	gcc parallel.c timing.c GetNumbers.c -o parallel.out -pthread

clean:
	rm *.csv *.out
