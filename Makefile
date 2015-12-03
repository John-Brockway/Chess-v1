CXX = g++
CXXFLAGS = -Wall -MMD -g
LDFLAGS = -lX11
EXEC = pp9k
OBJECTS = main.o board.o window.o piece.o player.o  graphicdisplay.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} ${LDFLAGS}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
