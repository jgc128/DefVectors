#ifndef KARAULOV_DEFINITION_CPP
#define KARAULOV_DEFINITION_CPP

#include <map>
#include <set>
#include <vector>
#include "definition.h"
#include "shareHeaders.h"

using namespace std;

extern set<unsigned long long> karaulov_set;

//initKaraulov(char* newName, std::map<Word *, int, WordCompare> &words)
void initKaraulov(vector<Definition*>* definitions, int count)
{
	static map<unsigned long long, int> totalWords;
	vector<Definition*>::iterator it;
	map<unsigned long long, int>::iterator jt;

	for (it = definitions->begin(); it != definitions->end(); it++)
	{
		for(jt = (*it)->mappedWords.begin(); jt != (*it)->mappedWords.end(); jt++)
		{
			totalWords[(*jt).first]++;
		}
	}

	for(jt = totalWords.begin(); jt != totalWords.end(); jt++)
	{
		if ((*jt).second <= count)
		{
			karaulov_set.insert((*jt).first);
		}
	}

	//1)Создать объект totalWords
	//2)Пройтись по всем дефинициям сразу, пополняя totalWords
	//3)А по те слова, которых больше определенного порога, добавить в karaulov_set
	
/*	std::map<Word *, int, WordCompare>::iterator it;

	for (it = mappedWords.begin(); it != mappedWords.end(); it++)
	{
		KaraulovDefinition::totalWords[(*it).first]++;
	}
*/
}


#endif
