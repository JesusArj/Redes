#include "partida.h"

partida::partida(int sd1, int sd2,string j1, string j2){
    setSockets(sd1,sd2);
    setJugadores(j1,j2);
    setRefranResuelto(pickRefran());
    setRefranOculto(ocultarRefran(this->refranResuelto));
    setTurno(0);
}

string partida::pickRefran()
{
    srand(time(NULL));
    string refran = Refran[rand() % 30];
    return refran;
}
string partida::ocultarRefran(string refran){
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
bool partida::resolverRefran(string refranIntento, string refran){
    bool intento = false;
    if(refran.compare(refranIntento))
        intento = true;
    return intento;
}
//TODO Revisar paso por referencia
bool partida::comprarVocales(string refranOculto, string refran, char vocal, int puntos ){
    bool cambio = false;
    char space = ' ', end = '\0';
    puntos -= 50;
    for(int i=0; i<refran.size(); i++){
        if(refran[i] == vocal){
            refranOculto[i]=vocal;
            cambio = true;
        }
    }
    return cambio;
}
//TODO Revisar paso por referencia
bool partida::rellenarConsonantes(string refranOculto, string refran, char consonante, int puntos ){
    int puntosOld = puntos;
    char space = ' ', end = '\0';
    for(int i=0; i<refran.size(); i++){
        if(refran[i] == consonante){
            refranOculto[i]=consonante;
            puntos += 50;
        }
    }
    if(puntosOld != puntos)
        return true;
    else
        return false;
}

int partida::getTurno(){
    return turno;
}
string partida::getRefranResuelto(){
    return refranResuelto;
}
string partida::getRefranOculto(){
    return refranOculto;
}
int* partida::getSockets(){
    return sockets;
}
string* partida::getJugadores(){
    return jugadores;
}

int* partida::getPuntos(){
    return puntos;
}

void partida::setTurno(int i){
    turno = i;
}
void partida::setRefranResuelto(string refran){
    refranResuelto = refran;
}
void partida::setRefranOculto(string refran){
    refranOculto = refran;
}
void partida::setSockets(int sd1, int sd2){
    sockets[0]=sd1;
    sockets[1]=sd1;
}
void partida::setJugadores(string jugador1, string jugador2){
    jugadores[0]=jugador1;
    jugadores[1]=jugador2;
}
void partida::setPuntos(int puntos1, int puntos2){
    puntos[0] = puntos1;
    puntos[1] = puntos2;
}

void partida::nextTurno(){
    int old = getTurno();
    turno = (old + 1) % 2;
}