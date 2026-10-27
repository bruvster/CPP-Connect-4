Compiler = g++
CFlags = -Wall -Wextra

run: build/AIPlayer.o build/game.o
	${Compiler} ${CFlags} build/AIPlayer.o build/game.o -o bin/run

build/AIPlayer.o: AIPlayer.cpp
	${Compiler} ${CFlags} -c -o build/AIPlayer.o AIPlayer.cpp 

build/game.o: game.cpp
	${Compiler} ${CFlags} -c -o build/game.o game.cpp 


clean:
	rm -f build/AIPlayer.o build/game.o build/game_new.o bin/run
