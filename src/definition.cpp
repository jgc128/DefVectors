#ifndef DEFINITION_CPP
#define DEFINITION_CPP

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include <set>

#include "definition.h"
#include "word.h"

pSimilarity Definition::similar = NULL;

Definition::Definition()
{

}

Definition::Definition(char* w, std::map<unsigned long long, int> &words){
	name = w;
	mappedWords = words;
	wordCount = mappedWords.size();
}

Definition::~Definition()
{

}

int Definition::Overlap(Definition &other, std::set<unsigned long long> &overlapDef)
{
	std::map<unsigned long long, int>::iterator it;

	for (it = mappedWords.begin(); it != mappedWords.end(); it++)
	{
		if (other.mappedWords.find(((*it).first)) != other.mappedWords.end())
		{
			overlapDef.insert(it->first);
		}
	}
	return overlapDef.size();
}

int Definition::Overlap(Definition &other)
{
	int count = 0;
	std::map<unsigned long long, int>::iterator it;

	for (it = mappedWords.begin(); it != mappedWords.end(); it++)
	{
		if (other.mappedWords.find(((*it).first)) != other.mappedWords.end())
		{
			count++;
		}
	}
	return count;
}

int Definition::Frequency(unsigned long long &word)
{
	std::map<unsigned long long, int>::const_iterator fnd = mappedWords.find(word);

	return fnd == mappedWords.end() ? 0 : mappedWords[word];	
}

#endif
