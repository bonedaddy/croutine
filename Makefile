build-go:
	cc -c -Wall ./src/go/go.c
	cc ./go.o -lpthread -o go_goroutine