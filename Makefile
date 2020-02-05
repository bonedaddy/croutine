build-go:
	cc -c -Wall ./test/croutine.c
	cc ./croutine.o -lpthread -o croutine