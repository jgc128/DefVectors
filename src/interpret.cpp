#ifndef INTERPERT_CPP
#define INTERPERT_CPP

#define PRINT_PARSE_INFO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <set>
#include <map>
#include <list>
#include <vector>

#include <algorithm>
#include <functional>

#include "word.h"
#include "definition.h"
#include "shareHeaders.h"

using namespace std;

extern map<char*, int, POSCompare> POS_map;
extern set<int> truePOS;

extern map < pair <char *, int >, unsigned long long, WordCompare > allWords;

extern GlobalArgs globalArgs;
extern map < unsigned long long, int> totalWordInDefinitions;

list<char *>* readFileWords(char*);
vector<Definition*>* readFileDefinitions(char*,list<char*>*,list<char*>*);
vector<Definition*>* readFileDefinitionsEx(char*,list<char*>*,list<char*>*);

unsigned long long getWordId(char * w, int pos)
{
	pair <char *, int> p = make_pair(w, pos);
	map < pair <char *, int >, unsigned long long, WordCompare >::iterator it;
	it = allWords.find(p);
	if (it != allWords.end())
		return (*it).second;

	unsigned long long s = allWords.size();
	allWords[p] = s;
	return s;
}

unsigned long long getWordId(pair <char *, int> p)
{
	map < pair <char *, int >, unsigned long long, WordCompare >::iterator it;
	it = allWords.find(p);
	if (it != allWords.end())
		return (*it).second;

	unsigned long long s = allWords.size();
	allWords[p] = s;
	return s;
}

void calculateTFIDF(vector<Definition*>* definitions)
{
	printMessage("Calculated TF*IDF...\n"); 
	int defSize = definitions->size();

	vector<Definition*>::iterator itDef;
	map<unsigned long long, int>::iterator itWord;

	float tfidf;
	
	for (itDef = definitions->begin(); itDef != definitions->end(); itDef++)
	{
		for(itWord = (*itDef)->mappedWords.begin(); itWord != (*itDef)->mappedWords.end(); itWord++)
		{
			tfidf = (((float)(*itWord).second) / (*itDef)->wordCount)*log(((float)defSize)/totalWordInDefinitions[(*itWord).first]);
		//	printf("TF = %f, IDF = %f, TF*IDF= %f\n", ((float)(*itWord).second) / (*itDef)->wordCount, ((float)defSize)/totalWordInDefinitions[(*itWord).first], tfidf);
			(*itWord).second = (int)(tfidf*0x0FFFFFF);
		}
	}
}

/// Normalize vectors with Unit Length: v = f / ||f||
void normalizeUnitLength(vector<Definition*>* definitions)
{
	printMessage("Normalizing vectors using Unit Length...\n"); 

	vector<Definition*>::iterator itDef;
	map<unsigned long long, int>::iterator itWord;

	unsigned long long length;
	double inv_length;
	double doubleValue;
	unsigned long long word;
	int freq;

	for (itDef = definitions->begin(); itDef != definitions->end(); itDef++)
	{
		// Calculate length of the vector
		length = 0;
		inv_length = 0;
		for(itWord = (*itDef)->mappedWords.begin(); itWord != (*itDef)->mappedWords.end(); itWord++)
		{
			word = (*itWord).first;
			freq = (*itDef)->Frequency( word );

			length += freq*freq;
		}
		inv_length = 1.0 / sqrt((double)length);

		for(itWord = (*itDef)->mappedWords.begin(); itWord != (*itDef)->mappedWords.end(); itWord++)
		{
			doubleValue = ((double)(*itWord).second) * inv_length;
			(*itWord).second = (int)(doubleValue*0x0FFFFFF);
		}
	}
}

/// Calulate Ge, it's need for Log-Entropy normalization
void calculateGe(vector<Definition*>* definitions, map<unsigned long long, double>* ge)
{
	vector<Definition*>::iterator itDef;
	map<unsigned long long, int>::iterator itWord;

	double logn = 1.0 / log( (double)definitions->size() );
	double sum;
	double div1;
	unsigned long long word;
	unsigned long long freq;

	for(itWord = totalWordInDefinitions.begin(); itWord != totalWordInDefinitions.end(); itWord++)
	{
		word = (*itWord).first;

		(*ge)[word] = 1.0;
		sum = 0.0;

		for (itDef = definitions->begin(); itDef != definitions->end(); itDef++)
		{
			freq = (*itDef)->Frequency(word);
			if(freq != 0)
			{
				div1 = freq / (double)totalWordInDefinitions[word];
				sum += ( div1 * log(div1) );
			}
		}

		(*ge)[word] += (logn * sum);
	}
}
/// Normalize vectors with Log-Entripy
void normalizeLogEntripy(vector<Definition*>* definitions)
{
	printMessage("Normalizing vectors using Log-Entropy...\n"); 

	vector<Definition*>::iterator itDef;
	map<unsigned long long, int>::iterator itWord;

	map<unsigned long long, double> *ge = new map<unsigned long long, double>();

	calculateGe(definitions, ge);

	double doubleValue;
	unsigned long long word;
	int freq;

	for (itDef = definitions->begin(); itDef != definitions->end(); itDef++)
	{
		for(itWord = (*itDef)->mappedWords.begin(); itWord != (*itDef)->mappedWords.end(); itWord++)
		{
			word = (*itWord).first;
			freq = (*itDef)->Frequency( word );

			doubleValue = log( freq + 1.0 ) * (*ge)[word];

			(*itWord).second = (int)(doubleValue*0x0FFFFFF);
		}
	}
}

void initWordsTotal(vector<Definition*>* definitions)
{
	vector<Definition*>::iterator it;
	map<unsigned long long, int>::iterator jt;

	for (it = definitions->begin(); it != definitions->end(); it++)
	{
		for(jt = (*it)->mappedWords.begin(); jt != (*it)->mappedWords.end(); jt++)
		{
			totalWordInDefinitions[(*jt).first]++;
		}
	}
}

vector<Definition*>* readData(char* wordFileName, char* definitionFileName, char* stopWordsFileName, char * posFileName = 0)
{
	if (posFileName)
	{
		bool firstPos = true;
		printMessage("Loading POS...\n");
		list<char *>* POSs = readFileWords(posFileName);

		printMessage("Using POS from file %s: ", posFileName);
		list < char * >::iterator it;
		for (it = POSs->begin(); it != POSs->end(); it++)
		{
			POS_add(*it);
			if (firstPos)
			{
				printf("%s", *it);
				firstPos = false;
			}
			else
				printf(", %s", *it);
		}
		printf("\n");
	}
	else
	{
		printMessage("Using default POS...\n");
		init_POS();
	}

	printMessage("Loading stop words...\n");
	list<char *>* stopWords = readFileWords(stopWordsFileName);
	printMessage("Stop  word loaded from file %s\n", stopWordsFileName);

	printMessage("Loading concepts...\n");
	list<char *>* concepts = readFileWords(wordFileName);
	printMessage("Concepts loaded from file %s\n", wordFileName);

	printMessage("Loading definitions...\n");
	vector<Definition*>* definitions = readFileDefinitionsEx(definitionFileName, stopWords, concepts);
	printMessage("Definitions loaded from file %s\n", definitionFileName);

	//print_deflist(definitions);
	
	return definitions;
}


list<char *>* readFileWords(char* fileName)
{
	FILE* file = fopen(fileName, "r");
	//if(file == NULL) //исключение        
	
	//Определяем размер
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	//Выделяем память под размер
	char* buf = new char[size + 1];
	
	//Читаем
	fread(buf, sizeof(char), size, file);
	
	
	list<char *>* stopWords;
	stopWords = new list<char *>();
	
	char* p = buf;
	char* pE;

	while( (pE = strchr(p, '\n')) != NULL )
	{
		*pE = '\0'; // make_pair

		stopWords->push_back(p);
		p = pE + 1;

	}
	
//	stopWords->push_back(p);
	fclose(file);

	return stopWords;
}

//Предикат
bool equeStr(char* a1, char* a2) { return strcmp(a1,a2) == 0; }

vector<Definition*>* readFileDefinitions(char* fileName, 
									  list<char*>* stopWords, 
									  list<char*>* concepts
									 )
{
	vector<Definition*>* defList = new vector<Definition*>(concepts->size() );
	
	FILE* file = fopen(fileName, "r");
	//if(!file) return false;
	
	//Определение размера файла
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);
	
	//Считывание всего файла
	char* text = new char[size + 1];
	//= new char[size + 1];
	
	size_t result = fread(text, 1, size, file);;
	if (result != size) 
	{
		if (ferror (file))
		{
			perror("Error reading file!");
			exit(3);
		}
		else
			size = result;
	}
	text[size] = '\0';
	

	//Интрепретация
	long i = 0;

	map<unsigned long long, int>* defWords;

	bool end = false;
	char* pb = text;
	char* pe;
	char* pc;
	char* ps1;
	char* ps2;
	char* pde;

	text += size;

	int f;
	int pos;

	do{

		if(*pb == '\n') { pb++; continue; }
		pe = strchr(pb, ';');
		if( pe == NULL ) break; //Документ окончен
		pc = pb;
		*pe = '\0';
		pb = pe + 1;

		if( find_if(concepts->begin(), concepts->end(), bind2nd(ptr_fun(equeStr), pc)) != concepts->end() )//Эта дефиниция требуется
		{
			defWords = new map<unsigned long long, int>();
			//it++;
			pde = strchr(pb, '\n');
			//if( pde == NULL ) break;
			*pde = '\0';
			do{
				if(*pb == ' ') { pb++; continue; }                
				
				ps1 = strchr(pb, '#');

				if( ps1 == NULL ) break; //Дефиниций окончена
	
				ps2 = strchr(ps1 + 1, '#');
				pe = strchr(ps2 + 1, ' ');
				*ps2 = '\0';
				*pe = '\0';
				pb = pe + 1;           

				pos = POS_map[ps1+1];
				if( pos != 0 && find_if(stopWords->begin(), stopWords->end(), bind2nd(ptr_fun(equeStr), ps2+1)) == stopWords->end()  )//Это слово требуется
				{
//printf("Words in Definition %s %s %d\n", ps1+1, ps2+1, pos);              
					//Это наша часть речи
				//	Word* word = new Word( ps2+1, pos);
					f = (*defWords)[getWordId(ps2+1, pos)]++;
				//	if( f > 1 ) delete word;                    
				}
			  }while( true );  
			pb = pde+1;
			(*defList)[i] = new Definition(pc, *defWords);
			i++;
		}
		else
		{
			pb = strchr(pb, '\n');
		}
	}while( pb < text );
	
	defList->resize(i);	

	return defList; 
}

/**
 * Performs parsing of a text file with definitions.
 * Each line of the file should be in the following format:
 * acacia; acacia#NN#acacia  is#VBZ#be  a#DT#a  genus#NN#genus  of#IN#of  shrubs#NNS#shrub
 * The definitions are separated by newline.
 * */
vector<Definition*>* readFileDefinitionsEx(char* def_filename, list<char*>* stopwords, list<char*>* concepts){
	// Initialize the definitions list
	vector<Definition*>* defList = new vector<Definition*>(concepts->size());

	// Read the whole definitions file into memory
	FILE* def_file = fopen(def_filename, "r");
	fseek(def_file, 0, SEEK_END);
	long size = ftell(def_file);
	rewind(def_file);
	char* text = new char[size + 1];
	size_t result = fread(text, 1, size, def_file);;
	if (result != size)	{
		if (ferror (def_file)) {
			perror("Error reading file!");
			exit(3);
		}
		else
			size = result;
	}
	text[size] = '\0';

	// Variables required for the parsing
	char* ptr_current = text;
	char* ptr_tokend;
	char* ptr_conend;
	char* ptr_concept;
	char* ptr_surface;
	char* ptr_lemma;
	char* ptr_defend;
	char* ptr_textend = text + size;
	bool use_definition;
	bool use_word;
	int pos;
	long def_count = 0;
	vector<Definition*>::iterator def_it;
	Definition* def;
	map<unsigned long long, int>* defWords;

	// Parse the file
	do{
		// Read the concept
		if(*ptr_current == '\n') { ptr_current++; continue; } // Go to the beginning of the definition
		ptr_conend = strchr(ptr_current, ';');
		if( ptr_conend == NULL ) break; // End of the document
		*ptr_conend = '\0';
		ptr_concept = ptr_current;
		ptr_current = ptr_conend + 1;

		use_definition = find_if(concepts->begin(), concepts->end(), bind2nd(ptr_fun(equeStr), ptr_concept)) != concepts->end();
		if(use_definition)
		{
			// If definition exist then use it, otherwise add a new entry
			int was = 0;
			def_it = find_if(defList->begin(), defList->end(), definitions_equal(new Definition(ptr_concept, *(new map<unsigned long long, int>()))));
			if(def_it != defList->end()) { defWords = &((*def_it)->mappedWords); was = (*def_it)->mappedWords.size();} // Definition already exist in the list
			else { defWords = new map<unsigned long long, int>(); }

			ptr_defend = strchr(ptr_current, '\n');
			*ptr_defend = '\0';
			do{
				// Get boundaries of the surface, pos, and lemma
				if(*ptr_current == ' ') {  ptr_current++; continue; } // Skip leading whitespaces
				if((ptr_surface = strchr(ptr_current, '#')) == NULL) break; // End of the definition
				if((ptr_lemma = strchr(ptr_surface + 1, '#')) == NULL) break; // Wrong format
				if((ptr_tokend = strchr(ptr_lemma + 1, ' ')) == NULL) ptr_tokend = ptr_defend; // End of the definiton
				*ptr_lemma = '\0';
				*ptr_tokend = '\0';
				pos = POS_map[ptr_surface+1];

				// Add word if not a stopword
				use_word = (pos != 0 && find_if(stopwords->begin(), stopwords->end(), bind2nd(ptr_fun(equeStr), ptr_lemma+1)) == stopwords->end());
				if(use_word)
				{
					(*defWords)[getWordId(ptr_lemma+1, pos)]++;
				}

				// Move to the next word
				ptr_current = ptr_tokend + 1;
				if (ptr_tokend == ptr_defend) break;
			} while(true);
			ptr_current = ptr_defend + 1;

			if(defWords->size() > 0)
			{
				if(def_it != defList->end())
				{
					printf("%s/%s: %d > %d\n", ptr_concept, (*def_it)->name, was, defWords->size());
					*def_it = new Definition(ptr_concept, *defWords);
				}
				else
				{
					(*defList)[def_count] = new Definition(ptr_concept, *defWords);
					def_count++;
				}
			}
		}
		else
		{
			ptr_current = strchr(ptr_current, '\n');
		}
	}while(ptr_current < ptr_textend);

	defList->resize(def_count);
	//printf("Definitions of %d(%d) concepts were loaded.\n", def_count, concepts->size());
	return defList;
}

bool result_sort (pair < unsigned long, pair < char*, char* > > &first, pair < unsigned long, pair < char*, char* > > &second)
{
	return second.first < first.first;
}


bool writeResults(list < pair < unsigned long, pair < char*, char* > > > *data, const char * fileName, bool rewrite)
{
	FILE* f = fopen(fileName, rewrite ? "w" : "a");
	if (!f)
		return false;

	data->sort(result_sort);

	list < pair < unsigned long, pair < char*, char* > > >::iterator it;
	for (it = data->begin(); it != data->end(); it++)
	{
		fprintf(f, "%s;%s;%f\n", (*it).second.first, (*it).second.second, (*it).first/10000000.0);
	}
	fclose(f);

	return true;
}


void createConceptsFromDefinitions(char * fileName, char * toFileName)
{
	char buffer[500];

	FILE* file = fopen(fileName, "r");
	FILE* to = fopen(toFileName, "w");

	char * pos;
	while(fgets(buffer, 500, file) != NULL)
	{
		if (buffer[0] == '\n')
			continue;
		pos = strchr(buffer, ';');

		if (pos == NULL)
			continue;

		*pos = '\0';

		fputs(buffer, to);
		fputc('\n', to);
	}

	fclose(file);
	fclose(to);
}

#endif
