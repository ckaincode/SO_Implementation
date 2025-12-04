

TARGET = main

all: $(TARGET)

main: main.cpp processo.h recursos.h
	g++  main.cpp -o main

run: main
	./main

# Remove o executavel
clean:
	rm -f main

.PHONY: all run clean
