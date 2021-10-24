#include "partida.h"

partida::partida(int sd1, int sd2,string j1, string j2){
    setSockets(sd1,sd2);
    setJugadores(j1,j2);
    setPuntos(0,0);
}

string Refran[] = {
#include "refranes"
} ;

void partida::pickRefran()
{
    srand(time(NULL));
    this->setRefranResuelto(Refran[rand() % 30]+'\n');
}
void partida::ocultarRefran(){
    string oculto;
    for(int i=0; i<refranResuelto.size(); i++){
        if(isalpha(refranResuelto[i])){
            oculto+="_";
        }else if(refranResuelto[i] == ' ' ){
            oculto+= " ";
        }
        else if(refranResuelto[i] == ',')
        {
            oculto+=","; 
        }
        else if(refranResuelto[i] == '\n'){
            oculto += "\n";
        }
    }
    refranOculto = oculto;
}
bool partida::resolverRefran(string refranIntento){
    bool intento = true;
    if(refranResuelto.compare(refranIntento)!=0)
    {
        intento=false; 
    }
    return intento;
}
bool partida::comprarVocales(char vocal, int sd){
    bool cambio = false;
    string oculto;
    puntos[sd] -= 50;
    for(int i=0; i<refranResuelto.size(); i++){
        if(refranResuelto[i] == vocal){
            oculto += vocal;
            cambio = true;
        }
        else{
            oculto += refranOculto[i];
        }
    }
    refranOculto = oculto;
    return cambio;
}
bool partida::comprobarVocales(char vocal)
{
    bool aux = false; 
    for(int i=0; i<refranResuelto.size(); i++){
        if(refranResuelto[i] == vocal){
            aux = true;
        }
    }
    return aux; 
}
bool partida::rellenarConsonantes(char consonante, int sd){
    int puntosOld = puntos[sd];
    string oculto;
    for(int i=0; i<refranResuelto.size(); i++){
        if(refranResuelto[i] == consonante){
            oculto += consonante;
            puntos[sd] += 50;
        }
        else{
            oculto += refranOculto[i];
        }
    }
    refranOculto = oculto;
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
    sockets[1]=sd2;
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
    if(turno==getSockets()[0])
    {
        turno=getSockets()[1]; 
    }
    else if (turno==getSockets()[1])
    {
        turno=getSockets()[0]; 
    }
}
