#ifndef SIMILARITY_H
#define SIMILARITY_H

#include "word.h"
#include "definition.h"

double Overlap(Definition &def1, Definition &def2);
double Cos(Definition &def1, Definition &def2);
double Karaulov(Definition &def1, Definition &def2);

#endif
