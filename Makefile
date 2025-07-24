CXX = g++-14
CXXFLAGS = -std=c++20 -Wall -MMD -g -Werror=vla
EXEC = chess
OBJECTS = test/main.o \
          Controller/controller.o \
          Model/board.o \
          Model/game.o \
          Model/piece.o \
          Model/player.o \
          Model/strategy.o \
          View/graphicalview.o \
          View/textview.o \
          View/window.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11 

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
