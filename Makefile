

TARGET = main

all: $(TARGET)

main: main.cpp processo.cpp recursos.cpp memoria.cpp escalonador.cpp arquivos.cpp setup.cpp
	g++ processo.cpp recursos.cpp memoria.cpp escalonador.cpp arquivos.cpp setup.cpp main.cpp -o main

run: main
	./main

# Remove o executavel
clean:
	rm -f main

.PHONY: all run clean