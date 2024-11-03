#Makefile 
# skriv 'make run' i terminalen for at køre programmet 

#Tjekker for om det er windows eller macOS 

ifeq ($(OS),Windows_NT) 
	TARGET = ./bin/a.exe else TARGET = ./bin/a.out 
endif 
	SRC = ./src/main.c 
	
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