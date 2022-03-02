CC=gcc
CFLAGS=-lSDL2 -lSDL2main -lSDL2_ttf -lSDL2_image -lm -o
CDEBUGFLAGS=-Wall -g
OUT=sdlrun
CLIBS=-L../DataContainers -lDataContainers
FILES=sdltestmain.c graphicsutils.c graphicsutils.h inpututils.c inpututils.h mathutils.c mathutils.h sdlfileutils.c sdlfileutils.h

compile:
	$(CC) $(FILES) $(CLIBS) $(CFLAGS) $(OUT)

linkedCompile:
	$(CC) sdltestmain.c -L. -lSoftUtils $(CLIBS) $(CFLAGS) $(OUT)

build:
	mkdir object
	$(CC) -c graphicsutils.c -o ./object/graphicsutils.o
	$(CC) -c inpututils.c -o ./object/inpututils.o
	$(CC) -c mathutils.c -o ./object/mathutils.o
	$(CC) -c sdlfileutils.c -o ./object/sdlfileutils.o
	ar rcs libSoftUtils.a ./object/*.o

destruct:
	rm -rf ./object
	rm -rf libSoftUtils.a

debug:
	$(CC) $(FILES) $(CLIBS) $(CDEBUGFLAGS) $(CFLAGS) $(OUT)

rebuild:
	make destruct
	make build
