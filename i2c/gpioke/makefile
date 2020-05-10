CFLAGS = -std=c11 -Wall -g
LIBS   = -lpci

gpioke: gpioke.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	
clean:
	rm gpioke
