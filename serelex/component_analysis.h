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


/// Function object for compare two string
struct pCharCompare:
public binary_function<const char*, const char*, bool>
{
	bool operator()(const char* c1, const char* c2) const
	{
		return strcmp(c1,c2) < 0;
	}
};

/// Predicate for compare first member of the pair and any number
struct p_fi_equal : 
public binary_function< pair < unsigned long long, unsigned long >, unsigned long long, bool> {
	bool operator() (const pair < unsigned long long, unsigned long > &a, const unsigned long long &b) const
	{
		bool res = a.first == b;
		return ( res );
	}
};


/// Represents class for component analysis
struct ComponentAnalysis
{
private:

/// Save KNN results
	static std::vector < nmax < pair < unsigned long long, unsigned long > > > NewKNN;

public:
/**
* Compare two pair
* @param a
* @param b
* @return -1 if a < b, 1 if a > b, 0 otherwise
*/
	static int compare(const pair < unsigned long long, unsigned long > &a, const pair < unsigned long long, unsigned long > &b);

/**
 * Prepare data for component analysis - calculate similairy and save KNN
 * @param D vector with need definitions
 */
	static void PrepareComponentAnalysis( std::vector<Definition*> &D );
/**
 * Returns results of component analysis with KNN
 * @param D vector with need definitions
 * @param R output list with results
 * @return list with results
 */
	static list < pair < char*, char* > > NewComponentAnalysisKNN(std::vector<Definition*> &D, std::list < pair < char*, char* > > &R);
/**
 * Returns results of component analysis with Mutual KNN
 * @param D vector with need definitions
 * @param R output list with results
 * @return list with results
 */
	static list < pair < char*, char* > > NewComponentAnalysisMutualKNN(std::vector<Definition*> &D, std::list<pair< char*, char* > > &R);

/// Pointer o need component analysis method	
	static pComponentAnalysisMethod RunComponentAnalysis;
};

#endif
