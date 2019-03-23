
CC = mpicc
CFLAGS = -g


t20s: t20.c
	mpicc -o t20s t20.c

t20c: t20.c
	mpicc -o t20c t20.c

t20:
	make t20s t20c

t23: t23.c
	mpicc -o t23 t23.c -lpthread

clean:
	rm -f  *.o
	rm -f  t00 t01 t02 t03 t04 t05 t06 t06a t06b
	rm -f  t07 t08 t09
	rm -f  t10 t11 t12 t13 t14 t15 t16 t17 t18 t19
	rm -f  t20s t20c
	rm -f  t20 t21 t22 t23 t24 t25 t26 t27 t28 t29
	rm -f  t30
