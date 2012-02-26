#ifndef DEFINITION_H
#define DEFINITION_H

#include <map>
#include <set>

#include <string.h>
#include "word.h"

struct Definition;

/// Pointer to the necessary similarity func
typedef double (* pSimilarity)(Definition&, Definition&);

/// Represents of the definition
struct Definition
{
	/*
	char* name;
	int wordCount;
	std::map<Word *, int, WordCompare> mappedWords;	
	*/
	char* name;
	
	/// count word in the definition
	int wordCount;

	/// key - unique ID of word, value - absolute frequency of word
	std::map<unsigned long long, int> mappedWords;

	//Указатель на конкретный метод определения схожести дефиниций
	//Установить при инициализации
	static pSimilarity similar;

	Definition();
	Definition(char * w, std::map<unsigned long long, int> &words);
	~Definition();

/**
 * Calc overlap betwwen this definition and other definition
 * @param other other definition
 * @return overlap two definiotn
 */
	int Overlap(Definition &other);

/**
 * Calc overlap betwwen this definition and other definition
 * @param other other definition
 * @param overlapDef set with common words
 * @return overlap two definiotn
 */
	int Overlap(Definition &other, std::set<unsigned long long> &overlapDef);
	
/**
 * Returns frequency word in defenition
 * @param word Word, witch returns the frequency
 * @return Frequency this word
 */	
	int Frequency(unsigned long long &word);
};


#endif
