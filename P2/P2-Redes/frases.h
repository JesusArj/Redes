#ifndef REFRANES
#define REFRANES

#include <string>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <algorithm>

using namespace std;

string Refran[] = {
#include "refranes"
} ;

string pickRefran();

string getRefranOculto(string refran);

bool rellenarConsonantes(string &refranOculto, string refran, char consonante, int &puntos );

bool comprarVocales(string &refranOculto, string refran, char vocal, int &puntos );

bool isVowel(char vocal);

bool isConsonant(char consonant);
#endif
