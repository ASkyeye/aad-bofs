BOFNAME := request_aad_prt
COMINCLUDE := -I ../common
LIBINCLUDE := -l ole32 -l winhttp
CC_x64 := x86_64-w64-mingw32-gcc
CC_x86 := i686-w64-mingw32-gcc

all:
	$(CC_x64) -o $(BOFNAME).x64.o $(COMINCLUDE) -Os -c entry.c -DBOF 
	$(CC_x86) -o $(BOFNAME).x86.o $(COMINCLUDE) -Os -c entry.c -DBOF # 32-bit untested

test:
	$(CC_x64) entry.c $(COMINCLUDE) $(LIBINCLUDE) -o $(BOFNAME).x64.exe
	$(CC_x86) entry.c $(COMINCLUDE) $(LIBINCLUDE) -o $(BOFNAME).x86.exe # 32-bit untested

check:
	cppcheck --enable=all $(COMINCLUDE) --platform=win64 entry.c --suppress=missingInclude

clean:
	rm $(BOFNAME).*.exe $(BOFNAME).*.o