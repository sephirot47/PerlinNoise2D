OPTIONS=-O2 -I/usr/include/pccts
	
all: main.exe

main.exe: obj/main.o
	g++ $(OPTIONS) obj/main.o -std=c++11 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system -o main.exe

obj/main.o: main.cpp
	g++ $(OPTIONS) -std=c++11 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system -c main.cpp -o obj/main.o
	
clean:
	rm -rf *.exe obj/*.o