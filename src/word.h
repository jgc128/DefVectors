#ifndef WORD_H
#define WORD_H

#pragma once

#include <stdio.h>
#include <string.h>
#include <set>
#include <map>

/// Represents of the word
struct Word
{	
	char* word;
	/// Part of speech
	int pos;

	Word(char*, int);
	~Word();
	
	bool operator == (Word &w);
	bool operator < (Word &w);
};

//=========================================================
/*
struct WordCompare:
public std::binary_function<const Word*, const Word*, bool>
{
	bool operator()(const Word* w1, const Word* w2) const
	{
	//	return w1->pos < w2->pos && strcmp(w1->word, w2->word) < 0;
		int i = strcmp(w1->word, w2->word);
		if (i == 0)
			return w1->pos < w2->pos;
		return i < 0;
	  //  return w1 < w2;
	}
};
*/

struct WordCompare:
public std::binary_function<const std::pair<char *, int >, const std::pair <char *, int >, bool>
{
	bool operator()(const std::pair <char *, int > w1, const std::pair <char *, int > w2) const
	{
	//	return w1->pos < w2->pos && strcmp(w1->word, w2->word) < 0;
		int i = strcmp(w1.first, w2.first);
		if (i == 0)
			return w1.second < w2.second;
		return i < 0;
	}
};


struct POSCompare:
public std::binary_function<const char*, const char*, bool>
{
	bool operator()(const char* w1, const char* w2) const
	{
		//printf("\"%s\"=?=\"%s\"\n", w1, w2);
		return strcmp(w1,w2) < 0;
	}
};
//=========================================================

//void init_truePOS();
//bool isTruePOS(char*);

void init_POS();//�?нициализация словаря всех частей речи
bool isPOS(char*);
int POS_add(char*);

#endif
