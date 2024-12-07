#Makefile 
# skriv 'make run' i terminalen for at køre programmet 

#Tjekker for om det er windows eller macOS 

TARGET = ./bin/a.out 

SRC = ./src/main.c
mkdir -p ./bin 
gcc main.c -o ./bin
./bin/a.out
#genererer a.out/a.exe fil og putter den i 'bin' 

$(TARGET): $(SRC) 
	mkdir -p ./bin 
	gcc $(SRC) -o $(TARGET) 

#'make run' kører programmet 

run: 
	$(TARGET) $(TARGET) 
	
#'make clean' giver mulighed for at slette bin 
clean: 
	rm -rf ./bin