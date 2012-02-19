#ifndef WORD_CPP
#define WORD_CPP

#include <stdio.h>
#include <string.h>

#include "word.h"

extern std::map<char*, int, POSCompare> POS_map;
//extern std::set<int> truePOS;

//Инициализация честей речи, которые интерпретируются
//Интерпретируемая часть речи должна быть полном списке частей речи
/*void init_truePOS()
{
    truePOS.insert( POS_map["VV"] );
    truePOS.insert( POS_map["VVN"] );
    truePOS.insert( POS_map["VVP"] );
    truePOS.insert( POS_map["JJ"] );
    truePOS.insert( POS_map["NN"] );
    truePOS.insert( POS_map["NNS"] );
    truePOS.insert( POS_map["NP"] );
}*/

//Инициализация полного списка частей речи
void init_POS()
{
    //POS_add("other");
    POS_add("VV");
    POS_add("VVN");
    POS_add("VVP");
    POS_add("JJJ");
    POS_add("NN");
    POS_add("NNS");
    POS_add("NP");
    //POS_add("SEND");
}

bool isPOS(char* pos)
{
    printf("isPOS:%s\n", pos);
    return POS_map.find(pos) != POS_map.end();
}

//Добавляет часть речи в полный список частей речи
int POS_add(char* strPOS)
{
    int i = POS_map.size() + 1;
    POS_map[strPOS] = i;
    return i;
}

/*bool isTruePOS(char* pos)
{    
    return true;
}*/

//========================================================

Word::Word(char* newWord, int newPOS)
{
//	word = new char[N];
	//word = new char[strlen(newWord) + 1];
	//strcpy(word, newWord);
	
	word = newWord;
	pos = newPOS;
}

Word::~Word()
{
	//delete this->word;
} 


bool Word::operator == (Word &w)
{
	return w.pos == this->pos && strcmp(w.word, this->word) == 0;
}
bool Word::operator < (Word &w)
{
	return this->pos < w.pos && strcmp(this->word,w.word) < 0;
}

#endif
