CFLAGS=-lSDL2 -lSDL2main -lSDL2_ttf -lSDL2_image -lm -o
OUT=sdlrun
FILES=sdltestmain.c graphicsutils.c graphicsutils.h inpututils.c inpututils.h mathutils.c mathutils.h sdlfileutils.c sdlfileutils.h ../Entity-Component-System/Type-Safe-Hash-Map/hashMap.c ../Entity-Component-System/Type-Safe-Hash-Map/hashMap.h

compile:
	gcc $(FILES) $(CFLAGS) $(OUT)

debug:
	gcc $(FILES) -g $(CFLAGS) $(OUT)
