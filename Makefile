all:
	gcc client.c -o client
	gcc edge.c -o edge
	gcc server_or.c -o server_or
	gcc server_and.c -o server_and
.PHONY: edge server_or server_and

edge:
	./edge
server_or:
	./server_or
server_and:
	./server_and
