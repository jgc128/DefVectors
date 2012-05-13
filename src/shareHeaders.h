#ifndef KR_2011_HEADERS_H
#define KR_2011_HEADERS_H

//============================================================
// файл с общими протатипами функций и глобальными перпеменными
//============================================================


#include <map>
#include <set>
#include <vector>
#include <list>

#include "word.h"
#include "definition.h"

void initKaraulov(std::vector<Definition*>* definitions, int count);
void printMessage(char * format, ...);
//Интерпретатор
std::vector<Definition*>* readData(char*, char*, char*, char*);
bool writeResults(std::list<std::pair <char*,char*> > *data, const char * fileName, bool rewrite = true);
void createConceptsFromDefinitions(char * fileName, char * toFileName);

enum Method {KNN, mKNN};

/// Represents common global variables
struct GlobalArgs
{
	/// Name of POS file
	char* posFile;
	/// Name of the stop-words file
    char* stopWordsFile;
	/// Name of the concepts file
    char* conceptsFile;
	/// Name of the definition file
    char* definitionsFile;
	/// Name of the output file
    char* outputFile;
    /// The method of choosen - KNN or Mutual KNN
    Method method;
	/// Number words for each words in results
    unsigned int K;
    int T1, T2, T3;
};

#endif
