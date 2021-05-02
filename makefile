CXX=g++
CXXFLAGS=-std=c++14 -Wall -MMD
VPATH=input:output:compression
EXEC=FlexiPress
OBJECTS= main.o ifile.o block.o huff.o bwt.o mtf.o rle.o stdin.o 
SRCDIR=.
OBJDIR=objs
SOURCES=$(shell find ${SRCDIR} -type f -name "*.cc")
OBJECTS:=${patsubst ${SRCDIR}/%.cc,${OBJDIR}/%.o,$(SOURCES)}
DEPENDS=${OBJECTS:.o=.d}
OUTDIRS=$(sort $(dir ${OBJECTS}))

${EXEC}:${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

${OBJDIR}/%.o:$(SRCDIR)/%.cc | ${OBJDIR} ${OUTDIRS}
	$(CXX) ${CXXFLAGS} -c -o $@ $<

${OUTDIRS}:
	mkdir $@

${OBJDIR}:
	mkdir $@

-include ${DEPENDS}

.PHONY:clean

clean:
	rm -r ${OBJDIR}
