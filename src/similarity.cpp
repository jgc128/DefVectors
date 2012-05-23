#ifndef SIMILARITY_CPP
#define SIMILARITY_CPP

#include <stdlib.h>
#include <stdio.h>

#include <map>
#include <set>
#include <math.h>

#include "similarity.h"
#include "definition.h"
#include "shareHeaders.h"
/*
extern struct GlobalArgs
{
	char* stopWordsFile;
	char* conceptsFile;
	char* definitionsFile;
	char* outputFile;
	//SymMethod symMethod;
	//Method method;
	int K;
	int T1, T2, T3;
};
*/
extern GlobalArgs globalArgs;
extern std::set<unsigned long long> karaulov_set;

double Overlap(Definition &def1, Definition &def2)
{
	int numShareWord = def1.Overlap(def2);
#ifdef I_AM_PARANOIK
	int as = def1.wordCount + def2.wordCount;
	if (!as)
		return 0;
	return numShareWord/((double)as);
#else
	return numShareWord/((double)(def1.wordCount + def2.wordCount));
#endif
}

double Cos(Definition &def1, Definition &def2)
{
	std::map<unsigned long long, int>::iterator itr;
	std::set<unsigned long long> words;
	std::set<unsigned long long>::iterator itrSet;

	double numerator = 0.0;
	double denominatorA = 0.0;
	double denominatorB = 0.0;

	int numeratorInt = 0;
	int denominatorAInt = 0;
	int denominatorBInt = 0;

	double a,b;
	int aInt,bInt;

	for(itr = def1.mappedWords.begin(); itr != def1.mappedWords.end(); ++itr)
		words.insert(itr->first);
	for(itr = def2.mappedWords.begin(); itr != def2.mappedWords.end(); ++itr)
		words.insert(itr->first);

	unsigned long long wrd;

	if (globalArgs.useTFIDF)
	{
		for(itrSet = words.begin(); itrSet != words.end(); ++itrSet)
		{
			wrd = *itrSet;

			a = ((double)def1.Frequency(wrd))/0x0FFFFFF;
			b = ((double)def2.Frequency(wrd))/0x0FFFFFF;

			numerator += a*b;
			denominatorA += a*a;
			denominatorB += b*b;
		}
	
		double denominator = sqrt((double)denominatorA) *  sqrt((double)denominatorB);
		return numerator / denominator;
	}
	else
	{
		for(itrSet = words.begin(); itrSet != words.end(); ++itrSet)
		{
			wrd = *itrSet;

			aInt = def1.Frequency(wrd);
			bInt = def2.Frequency(wrd);

			numeratorInt += aInt*bInt;
			denominatorAInt += aInt*aInt;
			denominatorBInt += bInt*bInt;
		}
	
		double denominator = sqrt((double)denominatorAInt) *  sqrt((double)denominatorBInt);
		return numeratorInt / denominator;
	}
}

double Karaulov(Definition &def1, Definition &def2)
{
	std::set<unsigned long long> overlapDef;
	std::set<unsigned long long>::iterator it;

	int numShareWord = def1.Overlap(def2,overlapDef);

	if(numShareWord >= globalArgs.T1)
		return 1;

	if(numShareWord >= globalArgs.T2)
	{
		for( it=overlapDef.begin(); it != overlapDef.end(); ++ it )
		{
			if ( karaulov_set.find(*it) == karaulov_set.end() )
				return 0;
		}
		return 1;
	}

	return 0;
}

#endif
