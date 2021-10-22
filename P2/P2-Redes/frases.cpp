#include "frases.h"

bool isVowel(char vocal){
    return (vocal=='A' || vocal=='E' || vocal=='I' || vocal=='O' || vocal=='U');
}

bool isConsonant(char consonant){
    return (!isVowel(consonant));
}