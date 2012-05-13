#ifndef DEFINITION_H
#define DEFINITION_H

#include <map>
#include <set>

#include <string.h>
#include "word.h"

struct Definition;

/// Pointer to the necessary similarity func
typedef double (* pSimilarity)(Definition&, Definition&);

/// Represents the definition
struct Definition
{
	/// Concept name
	char* name;
	
	/// Number of words in the definition
	int wordCount;

	/// Words in the definition. Key - unique ID of word, value - absolute frequency of word
	std::map<unsigned long long, int> mappedWords;

	//��������� �� ���������� ����� ����������� �������� ���������
	//���������� ��� �������������
	static pSimilarity similar;
	Definition();
	Definition(char* w);
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

struct definitions_equal
{
	explicit definitions_equal(Definition* d)
	{
		first_def = d;
	}

	bool operator()(const Definition* second_def) const
    {
		//printf("cmp ");
		if(first_def && second_def && first_def->name && second_def->name){
			return strcmp(first_def->name, second_def->name) == 0;
		}
		else{
			//printf("null ");
			return false;
		}
    }
private:
	Definition* first_def;
};



#endif
