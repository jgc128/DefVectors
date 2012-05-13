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

#define VERSION "1.1" 


#include "word.h"
#include "definition.h"
#include "similarity.h"
#include "shareHeaders.h"
#include "component_analysis.h"

#include "nmax.h"

/**
@mainpage Main
A tool for semantic relation extraction.
The program finds pairs of semantically related words based on the text definitions coming from the Wikipedia articles (other texts may be also used).
The extraction method implemented in this system is based on three similarity measures (cosine, gloss overlap, and Karaulov's measure) between texts and two nearest-neighbor algorithms (KNN and Mutual KNN).
The tool is a cross-platform console application.

Licensed under LGPLv3.
*/

map<char*, int, POSCompare> POS_map;//������������ ��������POS - ������
//set<int> truePOS;//��������� �������� POS, ������� ����������� � �������
set<unsigned long long> karaulov_set;

map < pair <char *, int >, unsigned long long, WordCompare> allWords;

GlobalArgs globalArgs;
static const char *optString = "p:c:d:s:o:S:M:K:T:h?:V";

bool argsAnalis(int, char**);
void helpMessage(char * fileName);
bool checkFiles();

extern int optind;

using namespace std;

// Print the definitions
void print_deflist(vector<Definition*>* defList){
	bool print_features = true;
	vector<Definition*>::iterator def_it;
	map<unsigned long long, int>::iterator words_it;
	for(def_it=defList->begin(); def_it !=defList->end(); def_it++){
		printf("%s", (*def_it)->name);
		if(print_features){
			printf(" (%d):", (*def_it)->mappedWords.size());
			//for(words_it=(*def_it)->mappedWords.begin(); words_it !=(*def_it)->mappedWords.end(); words_it++){
			//	printf("(%d,%d) ", (*words_it).first, (*words_it).second);
			//}
		}
		printf("\n");
	}
}

int main(int argc, char* argv[])
{
	globalArgs.posFile = 0;
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
	

	//������ ��������� ������
	if (argsAnalis(argc, argv))
	{
		return 1;
	}

	if (!checkFiles())
		return -1;
	
	printMessage("Started...\n");

	//�������������
	
	//init_truePOS();

	//���� ������
	std::vector<Definition*>* definitions = readData(
			globalArgs.conceptsFile, 
			globalArgs.definitionsFile, 
			globalArgs.stopWordsFile, 
			globalArgs.posFile); // readData("concepts.csv","definitions.csv","stoplist.csv")

	//print_deflist(definitions);

	initKaraulov(definitions, globalArgs.T3);

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
	printf("\n"
"c - concepts file, default concepts.csv. "
"A text file containing a set of input words (one word per line). "
"The program will try to find semantic relations between these input words. "
"For instance, if words �crocodile, alligator, house, and building� were given as input the program will try to return pairs �crocodile,alligator� and �house,building� among all possible combinations.\n");
	printf("\n"
"d - definitions file, default definitions.csv. "
"A text file containing a set of definitions for words specified in the concepts.csv file. "
"If this file contains definitions for more words that given in concepts.csv the program will skip the words which do not appear in the concepts.csv. "
"The sample-data directory contains files with \"definitions\" derived from the introduction of Wikipedia articles. "
"Other texts may be used as definitions (traditional dictionary glosses, WordNet, etc.) if provided in the same format.\n");
	printf("\n"
"s - stopWords file, default stoplist.csv. "
"A text file containing stop words (one word per line). "
"Words from this list will not be used by a similarity measure. "
"All occurrences of these words in definitons.csv will be removed.\n");
	printf("\n"
"o - outputfile, default result.csv. "
"A text file containing set of found semantic relations between words specified in the file concepts.csv. "
"Each line of this file contains a pair of semantically related words, according to the specified method.\n");
	printf("\n"
"S - similarity method, default o. Semantic similarity measure used for relation extraction.\n" 
"\to � Gloss overlap measure equal to number of common words in the definitions of two words.\n"
"\tc � Cosine between bag-of-word vectors build from definitions of respective of two words\n"
"\tk � Karaulov's semantic similarity measure.\n");
	printf("\n"
"M - component analysis method, default knn. An algorithm used to derive semantic relations from pairwise similarity scores between the words.\n"
"\t1 - knn. Standard nearest-neighbor algorithm (KNN). Here K most similar words are related to a target word.\n"
"\t2 - mknn. Mutual nearest-neighbor algorithm (MKNN). Here K mutually most similar words are related to a target word.\n");
	printf("\n"
"K - number of nearest-neighbors, default K = 2\n");
	printf("\n"
"T1, T2, T3 � parameters of Karaulov's semantic similarity measure, default T1 = 2, T1 = 1, T3 = 6.\n");
	printf("\n\nAuthors:\n");
	printf("\tAdeykin Sergey  adeykin90@gmail.com\n");
	printf("\tRomanov Alexey jgc128ra@gmail.com\n");
	printf("\tRomanov Pavel romanov4400@gmail.com\n");
}

void versionMessage()
{
	printf("Serelex %s\n",VERSION);
	printf("Copyright (C) 2012 Adeykin Sergey, Romanov Alexey and Romanov Pavel\n");
	printf("Licensed under LGPLv3 <http://www.gnu.org/licenses/lgpl.html>");
}

bool checkFile(char * fileName, bool writing = false) 
{
	FILE * file = fopen(fileName, writing ? "w" : "r");
	if (file != NULL) 
	{
		fclose(file); 
		return true;
	}
	printf("File %s doest not exsist!\n", fileName);
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
	//��������� �� ���������
	int opt = 0;
//	globalArgs.outputFile = "result.dat";

	//������
	opt = getopt( argc, argv, optString );
	while( opt != -1 ) 
	{
		switch( opt ) 
		{
		case 'p':
			globalArgs.posFile = optarg;
			break;
		case 'c'://���� ���������
			globalArgs.conceptsFile = optarg;
			break;
		case 'd'://���� ���������
			globalArgs.definitionsFile = optarg;
			break;
		case 's'://���� ����-����
			globalArgs.stopWordsFile = optarg;
			break;
		case 'o'://�������� ����
			globalArgs.outputFile = optarg;
			break;
		case 'S'://����� ����������� ��������
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
		case 'M'://�������� ���������� ������������� ���������
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
		case 'K'://���������� ��������� ��� ������� �� ����
			globalArgs.K = atoi(optarg);
			break;
		case 'T'://3 ��������� ��� ������ ���������
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
			helpMessage(argv[0]);
			return true; 
		case 'V':
			versionMessage();
			return true;
		}
		opt = getopt( argc, argv, optString );
	}

	return false;
}
