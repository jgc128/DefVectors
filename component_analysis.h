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
public binary_function< pair < unsigned long long, double >, unsigned long long, bool> {
	bool operator() (const pair < unsigned long long, double > &a, const unsigned long long &b) const
	{
		return ( a.first == b );
	}
};

struct ComponentAnalysis
{
private:

	static int compare(const pair < unsigned long long, double > &a, const pair < unsigned long long, double > &b);

	static std::vector < nmax < pair < unsigned long long, double > > > NewKNN;

public:
	static void PrepareComponentAnalysis( std::vector<Definition*> &D );

	static list < pair < char*, char* > > NewComponentAnalysisKNN(std::vector<Definition*> &D, std::list < pair < char*, char* > > &R);
	static list < pair < char*, char* > > NewComponentAnalysisMutualKNN(std::vector<Definition*> &D, std::list<pair< char*, char* > > &R);

	static pComponentAnalysisMethod RunComponentAnalysis;
};

#endif
