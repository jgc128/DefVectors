#ifndef DEFINITION_H
#define DEFINITION_H

#include <map>
#include <set>

#include <string.h>
#include "word.h"

struct Definition;
typedef double (* pSimilarity)(Definition&, Definition&);


struct Definition
{
	/*
	char* name;
	int wordCount;
	std::map<Word *, int, WordCompare> mappedWords;	
	*/
	char* name;
	int wordCount;

	// key - unique ID of word, value - absolute frequency of word
	std::map<unsigned long long, int> mappedWords;

	//Указатель на конкретный метод определения схожести дефиниций
	//Установить при инициализации
	static pSimilarity similar;

	Definition();
	Definition(char * w, std::map<unsigned long long, int> &words);
	~Definition();

	int Overlap(Definition &other);
	int Overlap(Definition &other, std::set<unsigned long long> &overlapDef);
	int Frequency(unsigned long long &word);
};


#endif
