#ifndef COMPONENTANALYSIS_CPP
#define COMPONENTANALYSIS_CPP

#define PRINT_PARSE_INFO

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <map>
#include <list>
#include <vector>
#include <functional>
#include <algorithm>

#include "component_analysis.h"
#include "definition.h"
#include "similarity.h"
#include "shareHeaders.h"


extern GlobalArgs globalArgs;

std::vector < nmax < pair < unsigned long long, unsigned long > > > ComponentAnalysis::NewKNN = std::vector < nmax < pair < unsigned long long, unsigned long > > > ();

pComponentAnalysisMethod ComponentAnalysis::RunComponentAnalysis = NULL;


int ComponentAnalysis::compare(const pair < unsigned long long, unsigned long > &a, const pair < unsigned long long, unsigned long > &b)
{
	long res = a.second - b.second;
	int ret = res < 0 ? -1 : (res > 0 ? 1 : 0);

	return ret;
}

void ComponentAnalysis::PrepareComponentAnalysis( std::vector<Definition*> &D )
{
	unsigned long long i,j,numC;
	double sim;
	unsigned long simL;

	numC = D.size();

	std::vector < pair < unsigned long long, unsigned long > > :: iterator it;

#ifdef	PRINT_PARSE_INFO
	unsigned long long total = ( (numC + 1) / 2 ) * numC;
	unsigned long long current = 0;
	double percent;

	printf("[00.00%%]");
#endif

	NewKNN.reserve( numC );
	for (i = 0; i < numC; ++ i)
		NewKNN.push_back( nmax < pair < unsigned long long, unsigned long > > ( globalArgs.K , ComponentAnalysis::compare ) );

	for (i = 0; i < numC; ++ i)
	{
		for (j = i; j < numC; ++ j)
		{
			if(i == j)
			{
			//	ComponentAnalysis::S(i,j) = 0;
			}
			else
			{
				sim =  Definition::similar( *D[i] , *D[j] );
				simL = sim * 10000000;
				if (simL != 0)
				{
					NewKNN[i].add( make_pair< unsigned long long, unsigned long > (j, simL) );
					NewKNN[j].add( make_pair< unsigned long long, unsigned long > (i, simL) );

				}
			}

#ifdef	PRINT_PARSE_INFO
			current++;
			if ((current&0xFFF) == 0)
			{
				percent = ( (double)current / total ) * 100;
				//printf("\r\tSimilaruty calc: %-10lld/%lld", current, total);
				printf("\b\b\b\b\b\b\b\b[%05.2f%%]", percent);
			}
#endif
		}
	}

#ifdef PRINT_PARSE_INFO	
	printf("\b\b\b\b\b\b\b\b[100%%]   \n");
#endif

}

list < pair < char*, char* > > ComponentAnalysis::NewComponentAnalysisKNN(std::vector<Definition*> &D, list < pair < char*, char* > > &R)
{
	int i,j,mi,mj;

	//mi = NewKNN.size();
	//for( i = 0; i < mi; ++i )
	//{
	//	mj = NewKNN[i].elements.size();
	//	for( j = 0; j < mj; ++j )
	//	{
	//		R.push_back( make_pair( D[ i ]->name, D[ NewKNN[i].elements[j].first ]->name ) );
	//	}
	//}

	mi = D.size();

	for( i = 0; i < mi; ++ i )
	{
		mj = NewKNN[i].elements.size();
		for( j = 0; j < mj; ++j )
		{
			R.push_back( make_pair( D[ i ]->name, D[ NewKNN[i].elements[j].first ]->name ) );
		}
	}

	return R;
}


list < pair < char*, char* > > ComponentAnalysis::NewComponentAnalysisMutualKNN(std::vector<Definition*> &D, list < pair < char*, char* > > &R)
{
	int word_i,word_j,mi,mj;
	std::vector < pair < unsigned long long, unsigned long > > :: iterator result;

	mi = D.size();

	for( word_i = 0; word_i < mi; ++ word_i )
	{
		mj = NewKNN[word_i].elements.size();
		for( word_j = 0; word_j < mj; ++ word_j )
		{

			result = find_if( 
				NewKNN[ NewKNN[word_i].elements[word_j].first ].elements.begin(), 
				NewKNN[ NewKNN[word_i].elements[word_j].first ].elements.end(), 
				bind2nd( p_fi_equal(), word_i ) );

			if( result != NewKNN[ NewKNN[word_i].elements[word_j].first ].elements.end() )
				R.push_back( make_pair( D[ word_i ]->name, D[ NewKNN[word_i].elements[word_j].first ]->name ) );
		}
	}

	return R;
}

#endif
