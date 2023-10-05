objects = main.o bmp.o music.o screen.o ts.o video.o 

edit: $(objects)
	${CC} -o edit $(objects) 

main.o: bmp.h music.h screen.h video.h ts.h 
	${CC} -c main.c

bmp.o: bmp.c bmp.h screen.c screen.h
	${CC} -c bmp.c

music.o: music.c music.h
	${CC} -c music.c

screen.o: screen.c screen.h
	${CC} -c screen.c

ts.o: ts.c ts.h
	${CC} -c ts.c

video.o: video.c video.h
	${CC} -c video.c

clean:
	rm edit $(objects)