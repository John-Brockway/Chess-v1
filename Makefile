CXX = g++
CXXFLAGS = -Wall -MMD -g
EXEC = pp9k
OBJECTS = main.o board.o window.o piece.o player.o  graphicdisplay.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
