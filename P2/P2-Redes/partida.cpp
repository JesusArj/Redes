#include "partida.h"

partida::partida(int sd1, int sd2,string j1, string j2){
    setSockets(sd1,sd2);
    setJugadores(j1,j2);
    pickRefran();
    ocultarRefran();
    setTurno(0);
    setPuntos(0,0);
}

void partida::pickRefran()
{
    srand(time(NULL));
    refranResuelto = Refran[rand() % 30];
}
void partida::ocultarRefran(){
    char aux = '_', space = ' ', end = '\0';
    for(int i=0; i<refranResuelto.size(); i++){
        if(isalpha(refranResuelto[i])){
            refranOculto[i]=aux;
        }else if(refranResuelto[i]==space){
            refranOculto[i]=space;
        }else if (refranResuelto[i]==end){
            refranOculto[i]==end;
        }
    }
}
bool partida::resolverRefran(string refranIntento){
    bool intento = false;
    if(refranResuelto.compare(refranIntento))
        intento = true;
    return intento;
}
//TODO Revisar paso por referencia
bool partida::comprarVocales(char vocal, int sd){
    bool cambio = false;
    char space = ' ', end = '\0';
    puntos[sd] -= 50;
    for(int i=0; i<refranResuelto.size(); i++){
        if(refranResuelto[i] == vocal){
            refranOculto[i]=vocal;
            cambio = true;
        }
    }
    return cambio;
}
//TODO Revisar paso por referencia
bool partida::rellenarConsonantes(char consonante, int sd){
    int puntosOld = puntos[sd];
    char space = ' ', end = '\0';
    for(int i=0; i<refranResuelto.size(); i++){
        if(refranResuelto[i] == consonante){
            refranOculto[i]=consonante;
            puntos[sd] += 50;
        }
    }
    if(puntosOld != puntos[sd])
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