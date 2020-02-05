build-go:
	cc -c -Wall ./test/go.c
	cc ./go.o -lpthread -o go_goroutine