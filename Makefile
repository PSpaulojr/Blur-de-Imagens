
main:
	gcc -omain main.c imageprocessing.c -I./ -lfreeimage
clean:
	rm main
	rm cachorro-out.jpg
execute:
	./main
	nomacs cachorro-out.jpg &
