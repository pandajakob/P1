call RM = rm -rf
call mkdir -p bin
call gcc -pedantic -Wall src/main.c -o ./bin/a.exe
call ./bin/a.exe