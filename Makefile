# Makefile
# Skriv 'make run' i terminalen for at køre programmet

# Tjekker for om det er Windows eller macOS

RM = rm -rf ./bin
MKDIR = mkdir -p bin
TARGET = bin/a.out
SRC = src/main.c

# Genererer a.out/a.exe fil og putter den i 'bin'
$(TARGET): $(SRC)
	$(RM)
    
	$(MKDIR)
	gcc -pedantic -Wall $(SRC) -o $(TARGET)

# 'make run' kører programmet
run: $(TARGET)
	$(TARGET) 0

runTest: $(TARGET)
	$(TARGET) debugTest1

