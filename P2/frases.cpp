#ifndef
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

string pickRefran(){
    srand(time(NULL));
    string refran = Refran[rand() % 30];
    return refran;
}

string getRefranOculto(string refran){
    string refranOculto(refran);
    char aux = '_', space = ' ', end = '\0';
    for(int i=0; i<refran.size(); i++){
        if(isalpha(refran[i])){
            refranOculto[i]=aux;
        }else if(refran[i]==space){
            refranOculto[i]=space;
        }else if (refran[i]==end){
            refranOculto[i]==end;
        }
    }
    return refranOculto;
}

bool rellenarConsonantes(string &refranOculto, string refran, char consonante, int &puntos ){
    int puntosOld = puntos;
    char space = ' ', end = '\0';
    for(int i=0; i<refran.size(); i++){
        if(refran[i] == consonante){
            refranOculto[i]=consonante;
            puntos += 50;
        }else if(refran[i]==space){
            refranOculto[i]=space;
        }else if (refran[i]==end){
            refranOculto[i]==end;
        }
    }
    if(puntosOld != puntos)
        return true;
    else
        return false;
}

bool comprarVocales(string &refranOculto, string refran, char vocal, int &puntos ){
    bool cambio = false;
    char space = ' ', end = '\0';
    puntos -= 50;
    for(int i=0; i<refran.size(); i++){
        if(refran[i] == vocal){
            refranOculto[i]=vocal;
            cambio = true;
        }else if(refran[i]==space){
            refranOculto[i]=space;
        }else if (refran[i]==end){
            refranOculto[i]==end;
        }
    }
    return cambio;
}

int main(){
    int puntos = 500;
    string refran = pickRefran();
    cout << refran <<endl;
    string refranOculto = getRefranOculto(refran);
    cout << refranOculto<<endl;
    char l = 'L';
    char a = 'A';
    rellenarConsonantes(refranOculto, refran, l , puntos );
    comprarVocales(refranOculto,refran,a,puntos);
    cout << refranOculto << " puntos "<< puntos <<endl;
    return 0;
}

#endif
