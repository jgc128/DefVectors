ifndef ARCH
ARCH = m32
endif

CC = g++ -$(ARCH)

serelex: ./src/main.o ./src/word.o ./src/definition.o ./src/interpret.o ./src/similarity.o ./src/karaulov.o ./src/component_analysis.o
	$(CC) -O2 -o serelex ./src/main.o ./src/word.o ./src/definition.o ./src/interpret.o ./src/similarity.o ./src/karaulov.o ./src/component_analysis.o
#serelex: main.o
#	$(CC) -o serelex main.o

main.o: ./src/main.cpp ./src/shareHeaders.h ./src/nmax.h
word.o: ./src/word.cpp ./src/word.h 
interpret.o: ./src/interpret.cpp ./src/shareHeaders.h
definition.o: ./src/definition.cpp
similarity.o: ./src/similarity.cpp
karaulov.o: ./src/karaulov.cpp ./src/word.h
component_analysis.o: ./src/component_analysis.cpp ./src/component_analysis.h

#word.cpp: word.h
#similarity.cpp: similarity.h
#definition.cpp: definition.h

clean:
	rm -f ./src/main.o ./src/word.o ./src/similarity.o ./src/definition.o ./src/interpret.o ./src/karaulov.o ./src/component_analysis.o
