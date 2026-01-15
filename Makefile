build:
	@gcc -g *.c -o runic -lm

clean:
	@rm -f runic

pack:
	zip -FSr 314CD_VladGeorgeCacenschi_Tema3.zip README Makefile *.c *.h