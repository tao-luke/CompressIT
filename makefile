CXX=g++
CXXFLAGS=-std=c++14 -Wall -MMD
VPATH=input:output:compression
EXEC=FlexiPress
OBJECTS= main.o ifile.o block.o huff.o bwt.o mtf.o rle.o stdin.o 
DEPENDS=${OBJECTS:.o=.d}

${EXEC}:${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}
	
-include ${DEPENDS}

.PHONY:clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
