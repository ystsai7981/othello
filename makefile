othello.exe: gui.o board.o algorithm.o main.cpp
	g++ gui.o board.o algorithm.o main.cpp -o othello.exe
algorithm.o: algorithm.cpp algorithm.h
	g++ -c algorithm.cpp algorithm.h
board.o: board.cpp board.h
	g++ -c board.cpp board.h
gui.o: gui.cpp gui.h
	g++ -c gui.cpp gui.h

clean:
	rm -f board.o board.h.gch gui.o gui.h.gch othello.exe