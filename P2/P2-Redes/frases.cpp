#include "frases.h"

bool isVowel(char vocal){
    return (vocal=='a' || vocal=='e' || vocal=='i' || vocal=='o' || vocal=='u');
}

bool isConsonant(char consonant){
    return (!isVowel(consonant));
}