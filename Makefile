sock_pair:sock_pair.c
	gcc -o $@ $^
.PHONY:clean
clean:
	rm -f sock_pair
