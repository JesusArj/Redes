#include "partida.h"

partida::partida(int sd1, int sd2,string j1, string j2){
    setSockets(sd1,sd2);
    setJugadores(j1,j2);
    setRefranResuelto(pickRefran());
    setRefranOculto(ocultarRefran(this->refranResuelto));
    setTurno(0);
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

void partida::nextTurno(){
    int old = getTurno();
    turno = (old + 1) % 2;
}