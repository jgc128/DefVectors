#ifndef INTERPERT_CPP
#define INTERPERT_CPP

#define PRINT_PARSE_INFO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

list<char *>* readFileWords(char*);
vector<Definition*>* readFileDefinitions(char*,list<char*>*,list<char*>*);

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

vector<Definition*>* readData(char* wordFileName, char* definitionFileName, char* stopWordsFileName)
{
	list<char *>* stopWords = readFileWords(stopWordsFileName);
	printMessage("Stow word loaded from file %s\n", stopWordsFileName);

	list<char *>* concepts = readFileWords(wordFileName);
	printMessage("Concepts loaded from file %s\n", wordFileName);

	vector<Definition*>* definitions = readFileDefinitions(definitionFileName, stopWords, concepts);    
	printMessage("Definitions loaded from file %s\n", definitionFileName);

	//delete stopWords;
	//delete concepts;
	
	return definitions;
	
	//printf("Count=%i\n", definitions->size() );
	//for(vector<Definition*>::iterator i = definitions->begin(); i != definitions->end(); i++)
	//    printf("+");
	
//list<char*>::iterator i;
//for(i = concepts->begin(); i != concepts->end(); i++ )
//    printf("-%s\n", *i);
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

bool writeResults(list < pair < char*, char* > > *data, const char * fileName, bool rewrite)
{
	FILE* f = fopen(fileName, rewrite ? "w" : "a");
	if (!f)
		return false;

	list < pair < char*, char* > >::iterator it;
	for (it = data->begin(); it != data->end(); it++)
	{
		fprintf(f, "%s;%s\n", (*it).first, (*it).second);
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
