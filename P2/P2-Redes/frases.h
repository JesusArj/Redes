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

bool isVowel(char vocal);

bool isConsonant(char consonant);
#endif
