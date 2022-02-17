CFLAGS=-lSDL2 -lSDL2main -lSDL2_ttf -lSDL2_image -lm -o
CDEBUGFLAGS=-Wall -g
OUT=sdlrun
CLIBS=-LDataContainers -lDataContainers
FILES=sdltestmain.c graphicsutils.c graphicsutils.h inpututils.c inpututils.h mathutils.c mathutils.h sdlfileutils.c sdlfileutils.h

compile:
	gcc $(FILES) $(CLIBS) $(CFLAGS) $(OUT)

debug:
	gcc $(FILES) $(CLIBS) $(CDEBUGFLAGS) $(CFLAGS) $(OUT)
