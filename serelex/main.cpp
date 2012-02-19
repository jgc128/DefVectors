#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include <vector>
#include <map>
#include <set>

#ifdef _WIN32
	#include "getopt.h"
#else
	#include <getopt.h>
#endif

#pragma once

#define I_AM_PARANOIK
#define PRINT_PARSE_INFO

#include "word.h"
#include "definition.h"
#include "similarity.h"
#include "shareHeaders.h"
#include "component_analysis.h"

#include "nmax.h"

/**
@mainpage Main
Serelex - program complex for extracting semantic relations with the help of the nearest-neighbor algorithm.

Licensed under LGPLv3.
*/

map<char*, int, POSCompare> POS_map;//Соответствие НазваниеPOS - Индекс
//set<int> truePOS;//Множество индексов POS, которые учитываются в анализе
set<unsigned long long> karaulov_set;

map < pair <char *, int >, unsigned long long, WordCompare> allWords;

GlobalArgs globalArgs;
static const char *optString = "c:d:s:o:S:M:K:T:h?";

bool argsAnalis(int, char**);
void helpMessage(char * fileName);
bool checkFiles();

extern int optind;

using namespace std;

int main(int argc, char* argv[])
{
	globalArgs.conceptsFile = "concepts.csv";
	globalArgs.definitionsFile = "definitions.csv";
	globalArgs.stopWordsFile = "stoplist.csv";
	globalArgs.outputFile = "result.csv";
	globalArgs.K = 2;
	globalArgs.T1 = 2;
	globalArgs.T2 = 1;
	globalArgs.T3 = 6;

	Definition::similar = Overlap;
	ComponentAnalysis::RunComponentAnalysis = ComponentAnalysis::NewComponentAnalysisKNN;
	

	//Анализ командной строки
	if (argsAnalis(argc, argv))
	{
		helpMessage(argv[0]);
		return 1;
	}

	if (!checkFiles())
		return -1;
	
	printMessage("Started...\n");

	//Инициализация
	init_POS();
	//init_truePOS();

	//Ввод данных
	std::vector<Definition*>* definitions = readData(globalArgs.conceptsFile, globalArgs.definitionsFile, globalArgs.stopWordsFile); // readData("concepts.csv","definitions.csv","stoplist.csv")
	
	initKaraulov( definitions, globalArgs.T3 );

	list < pair < char*,char* > > result;
	
	printMessage("Calculated similarity... "); 
	
	ComponentAnalysis::PrepareComponentAnalysis( *definitions );
	ComponentAnalysis::RunComponentAnalysis(*definitions,result);

	if (Definition::similar == Karaulov)
	{
		printMessage("Similarity calculated with Karaulov method (T1: %d, T2: %d, T3: %d) \n", globalArgs.T1, globalArgs.T2, globalArgs.T3);
	}
	else
	{
		printMessage("Similarity calculated with %s method \n", 
			Definition::similar == Overlap ? "Overlap" :  "Cosinus"
		);
	}
	
	writeResults(&result, globalArgs.outputFile);
	
	printMessage("Completed! Writed result to file %s\n", globalArgs.outputFile);

	return 0;
}

void printMessage(char * format, ...)
{
	char buffer[256];

	time_t rawtime;
	struct tm * timeinfo;
	char timeStr[30];
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime(timeStr, 30, "%d.%m.%Y %H:%M:%S",timeinfo);

	va_list args;
	va_start(args, format);
	vsprintf(buffer,format, args);
	va_end(args);

	fprintf(stderr, "[%s] %s", timeStr, buffer);
}

void helpMessage(char * fileName)
{
	printf("\nUsage: %s [-c <file>] [-d <file>] [-s <file>] [-o <file>] [-S o|c|k] [-M 1|2] [-K <K>] [-T <T1> <T2> <T3>]\n", fileName);
	printf("c - concepts file, default concepts.csv\n");
	printf("d - definitions file, default definitions.csv\n");
	printf("s - stopWords file, default stoplist.csv\n");
	printf("o - outputfile, default result.csv\n");
	printf("S - similarity method, default o\n");
	printf("\to - OverWords\n");
	printf("\tc - Cos\n");
	printf("\tk - Karaulov method\n");
	printf("M - component analysis method, default 1\n");
	printf("\t1 - KNN\n");
	printf("\t2 - mKNN\n");
	printf("K, T1, T2, T3 - factors, default K = 2, T1 = 2, T1 = 1, T3 = 6\n");
	printf("Authors:\n");
	printf("\tAdeykin Sergey  adeykin90@gmail.com\n");
	printf("\tRomanov Alexey jgc128ra@gmail.com\n");
	printf("\tRomanov Pavel romanov4400@gmail.com\n");
}

bool checkFile(char * fileName, bool writing = false) 
{
	FILE * file = fopen(fileName, writing ? "w" : "r");
	if (file != NULL) 
	{
		fclose(file); 
		return true;
	}
	printf("File %s doest not exsist!", fileName);
	return false;
}

bool checkFiles()
{
	return 
			checkFile(globalArgs.conceptsFile) && 
			checkFile(globalArgs.definitionsFile) && 
			checkFile(globalArgs.stopWordsFile) && 
			checkFile(globalArgs.outputFile, true);
}

bool argsAnalis(int argc, char* argv[])
{
	//Параметры по умолчанию
	int opt = 0;
//	globalArgs.outputFile = "result.dat";

	//Анализ
	opt = getopt( argc, argv, optString );
	while( opt != -1 ) 
	{
		switch( opt ) 
		{
		case 'c'://файл концептов
			globalArgs.conceptsFile = optarg;
			break;
		case 'd'://Файл дефиниций
			globalArgs.definitionsFile = optarg;
			break;
		case 's'://Файл стоп-слов
			globalArgs.stopWordsFile = optarg;
			break;
		case 'o'://Выходной файл
			globalArgs.outputFile = optarg;
			break;
		case 'S'://Метод определения схожести
			switch(*optarg)
			{
			case 'o':
				Definition::similar = Overlap;
				break;
			case 'c':
				Definition::similar = Cos;
				break;
			case 'k':
				Definition::similar = Karaulov;
				break;
			}
			break;
		case 'M'://Алгоритм вычисления семантических отношений
			switch(*optarg)
			{
			case '1':
				//globalArgs.method = KNN;
				ComponentAnalysis::RunComponentAnalysis = ComponentAnalysis::NewComponentAnalysisKNN;
				break;
			case '2':
				//globalArgs.method = mKNN;
				ComponentAnalysis::RunComponentAnalysis = ComponentAnalysis::NewComponentAnalysisMutualKNN;
				break;
			}
			break;
		case 'K'://Количество отношений для каждого из слов
			globalArgs.K = atoi(optarg);
			break;
		case 'T'://3 параметра для метода караулова
			if (optind + 1 < argc)
			{
				globalArgs.T1 = atoi( optarg );
				globalArgs.T2 = atoi( argv[optind] );
				globalArgs.T3 = atoi( argv[optind+1] );
				optind += 2;
				break;
			}
		case 'h':
		case '?':
			return true;        
		}
		opt = getopt( argc, argv, optString );
	}

	return false;
}
