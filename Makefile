CC=gcc
CCWIN=x86_64-w64-mingw32-gcc
CFLAGS=-lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lm -o
CDEBUGFLAGS=-Wall -g
OUT=sdlrun
FILES=sdltestmain.c graphicsutils.c hashMap.h hashMap.c graphicsutils.h inpututils.c inpututils.h mathutils.c mathutils.h sdlfileutils.c sdlfileutils.h

compile:
	clear
	$(CC) $(FILES) $(CFLAGS) $(OUT)

debug:
	clear
	$(CC) $(FILES) $(CDEBUGFLAGS) $(CFLAGS) $(OUT)

debug-win:
	clear
	rm -rf build-win/
	mkdir build-win/
	$(CCWIN) $(FILES) $(CDEBUGFLAGS) -ISDL2/include/ -LSDL2/lib/ -lmingw32 $(CFLAGS) $(OUT).exe
	mv $(OUT).exe build-win/
	cp SDL2_image.dll build-win/
	cp SDL2.dll build-win/
	cp SDL2_ttf.dll build-win/

clean:
	rm -f $(OUT)
	rm -rf build-win/
