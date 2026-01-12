build:
	@gcc -g *.c -o runic -lm

clean:
	@rm -f runic