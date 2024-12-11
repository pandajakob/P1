# Makefile
# Skriv 'make run' i terminalen for at køre programmet

# Tjekker for om det er Windows eller macOS
ifeq ($(OS),Windows_NT)
    RM = del /Q /F
    MKDIR = if not exist bin mkdir bin
    TARGET = bin\a.exe
else
    RM = rm -rf
    MKDIR = mkdir -p bin
    TARGET = bin/a.out
endif

SRC = src/main.c

# Genererer a.out/a.exe fil og putter den i 'bin'
$(TARGET): $(SRC)
	$(MKDIR)
	gcc -pedantic -Wall $(SRC) -o $(TARGET)

# 'make run' kører programmet
run: $(TARGET)
	$(TARGET)

# 'make clean' giver mulighed for at slette bin
clean:
	$(RM) bin