CC = g++

serelex: main.o word.o definition.o interpret.o similarity.o karaulov.o component_analysis.o
	$(CC) -O2 -o serelex main.o word.o definition.o interpret.o similarity.o karaulov.o component_analysis.o
#serelex: main.o
#	$(CC) -o serelex main.o

main.o: main.cpp shareHeaders.h nmax.h
word.o: word.cpp word.h 
interpret.o: interpret.cpp shareHeaders.h
definition.o: definition.cpp
similarity.o: similarity.cpp
karaulov.o: karaulov.cpp word.h
component_analysis.o: component_analysis.cpp component_analysis.h

#word.cpp: word.h
#similarity.cpp: similarity.h
#definition.cpp: definition.h

clean:
	rm -f main.o word.o similarity.o definition.o interpret.o karaulov.o component_analysis.o
