
all: nweb  client

nweb: nweb23.c
	gcc -O3 -static nweb23.c -o nweb

client: client.c
	gcc -O3 -static client.c -o client
	
clean: 
	rm -rf nweb client
