#ifndef COMPONENTANALYSIS_H
#define COMPONENTANALYSIS_H

#include <map>
#include <list>
#include <vector>

#include "word.h"
#include "definition.h"
#include "nmax.h"

using namespace std;

typedef list<pair < char*,char* > > (* pComponentAnalysisMethod)(std::vector<Definition*> &D, std::list< pair < char*,char* > > &R);

struct pCharCompare:
public binary_function<const char*, const char*, bool>
{
	bool operator()(const char* c1, const char* c2) const
	{
		return strcmp(c1,c2) < 0;
	}
};
struct p_fi_equal : 
public binary_function< pair < unsigned long long, unsigned long >, unsigned long long, bool> {
	bool operator() (const pair < unsigned long long, unsigned long > &a, const unsigned long long &b) const
	{
		bool res = a.first == b;
		return ( res );
	}
};

struct ComponentAnalysis
{
private:

	static std::vector < nmax < pair < unsigned long long, unsigned long > > > NewKNN;

public:
	static int compare(const pair < unsigned long long, unsigned long > &a, const pair < unsigned long long, unsigned long > &b);

	static void PrepareComponentAnalysis( std::vector<Definition*> &D );

	static list < pair < char*, char* > > NewComponentAnalysisKNN(std::vector<Definition*> &D, std::list < pair < char*, char* > > &R);
	static list < pair < char*, char* > > NewComponentAnalysisMutualKNN(std::vector<Definition*> &D, std::list<pair< char*, char* > > &R);

	static pComponentAnalysisMethod RunComponentAnalysis;
};

#endif
