#ifndef KR_2011_HEADERS_H
#define KR_2011_HEADERS_H

//============================================================
//файл с общими протатипами функций и глобальными перпеменными
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
std::vector<Definition*>* readData(char*, char*, char*);
bool writeResults(std::list<std::pair <char*,char*> > *data, const char * fileName, bool rewrite = true);
void createConceptsFromDefinitions(char * fileName, char * toFileName);

enum Method {KNN, mKNN};

//Структура с глобальными параметрами
struct GlobalArgs
{
    char* stopWordsFile;
    char* conceptsFile;
    char* definitionsFile;
    char* outputFile;
    //SymMethod symMethod;
    Method method;
    unsigned int K;
    int T1, T2, T3;
};

#endif
