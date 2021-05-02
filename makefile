CXX=g++
CXXFLAGS=-std=c++14 -Wall -MMD
VPATH=input:output:compression
EXEC=FlexiPress
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
