CC = g++

serelex: main.o word.o definition.o interpret.o similarity.o karaulov.o component_analysis.o
	$(CC) -O2 -o serelex main.o word.o definition.o interpret.o similarity.o karaulov.o component_analysis.o
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
	rm -f main.o word.o similarity.o definition.o interpret.o karaulov.o component_analysis.o
