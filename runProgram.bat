call RM = rm -rf ./bin
call mkdir -p bin
call gcc -pedantic -Wall src/main.c -o ./bin/a.exe
call ./bin/a.exe 0